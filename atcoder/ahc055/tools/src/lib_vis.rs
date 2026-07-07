use svg::node::element::{Rectangle, Text, SVG};
use svg::node::Text as TextNode;

use crate::{Input, Op, VisData};

const W: f64 = 900.0;
const H: f64 = 900.0;
const PADDING: f64 = 20.0;

pub fn new_svg() -> SVG {
    svg::Document::new()
        .set("id", "vis")
        .set("viewBox", (0.0, 0.0, W + 2.0 * PADDING, H + 2.0 * PADDING))
        .set("width", W + 2.0 * PADDING)
        .set("height", H + 2.0 * PADDING)
}

const TOTAL_PADDING_WIDTH: f64 = 40.0;
const TOTAL_PADDING_HEIGHT: f64 = 80.0;
const N_COLS: usize = 6;

pub fn draw_treasure_grid(
    vis_data: &VisData,
    input: &Input,
    attacker_weapon_index: Option<usize>,
    mut doc: SVG,
) -> SVG {
    let cols = N_COLS; // Increased columns for wider canvas
    let rows = (input.n + cols - 1) / cols;

    let cell_width = (W - TOTAL_PADDING_WIDTH) / cols as f64;
    let cell_height = (H - TOTAL_PADDING_HEIGHT) / rows as f64;
    let start_x = PADDING;
    let start_y = PADDING;

    for i in 0..input.n {
        let col = i % cols;
        let row = i / cols;
        let x = start_x + col as f64 * cell_width + TOTAL_PADDING_WIDTH * col as f64 / cols as f64;
        let y =
            start_y + row as f64 * cell_height + TOTAL_PADDING_HEIGHT * row as f64 / rows as f64;

        if vis_data.state.treasure_opened[i] {
            // Open treasure chest with weapon status
            doc = draw_treasure_chest_cell(doc, vis_data, i, x, y, cell_width, cell_height);

            // Check if this treasure was just opened this turn (HP became 0)
            let just_opened = if let Some(ref op) = vis_data.op {
                let Op::Attack(attack) = op;
                attack.target_treasure_index == i && vis_data.state.treasure_hitpoints[i] <= 0
            } else {
                false
            };

            if just_opened {
                doc = draw_hp_zero(doc, vis_data, input, i, x, y, cell_width, cell_height);
            } else {
                doc = draw_weapon_usage_gauge(doc, vis_data, i, x, y, cell_width, cell_height);
            }
        } else {
            // Closed treasure chest with HP gauge
            doc = draw_treasure_chest_cell(doc, vis_data, i, x, y, cell_width, cell_height);
            doc = draw_hp_gauge(
                doc,
                vis_data,
                input,
                i,
                x,
                y,
                cell_width,
                cell_height,
                attacker_weapon_index,
            );
        }
    }

    doc
}

fn draw_treasure_chest_cell(
    mut doc: SVG,
    vis_data: &VisData,
    i: usize,
    x: f64,
    y: f64,
    cell_width: f64,
    cell_height: f64,
) -> SVG {
    let (cell_color, border_color) = if vis_data.state.treasure_opened[i] {
        ("#ccffcc", "#00aa00") // Light green background, dark green border
    } else {
        ("white", "#999") // White background, light gray border
    };

    // Check if this treasure was attacked in the current turn
    let (final_border_color, stroke_width) = if let Some(ref op) = vis_data.op {
        let Op::Attack(attack) = op;
        if attack.target_treasure_index == i {
            ("#ff0000", 3.0) // Red border with thick stroke for attacked treasure
        } else if !attack.is_hand && attack.weapon_index == i {
            ("#ffa500", 3.0) // Orange border with thick stroke for used weapon
        } else {
            (border_color, 1.0)
        }
    } else {
        (border_color, 1.0)
    };

    doc = doc.add(
        Rectangle::new()
            .set("x", x)
            .set("y", y)
            .set("rx", 2.0)
            .set("ry", 2.0)
            .set("width", cell_width)
            .set("height", cell_height)
            .set("fill", cell_color)
            .set("stroke", final_border_color)
            .set("stroke-width", stroke_width),
    );

    // Check if this treasure is being attacked
    let text_color = if let Some(ref op) = vis_data.op {
        let Op::Attack(attack) = op;
        if attack.target_treasure_index == i {
            "#ff0000" // Red color for attacked treasure
        } else {
            "#333"
        }
    } else {
        "#333"
    };

    doc = doc.add(
        Text::new()
            .set("x", x + 5.0)
            .set("y", y + 17.0)
            .set("font-family", "monospace")
            .set("font-size", "14")
            .set("font-weight", "bold")
            .set("fill", text_color)
            .add(TextNode::new(format!("{}", i))),
    );

    doc
}

fn draw_hp_zero(
    mut doc: SVG,
    vis_data: &VisData,
    input: &Input,
    i: usize,
    x: f64,
    y: f64,
    _cell_width: f64,
    _cell_height: f64,
) -> SVG {
    doc = doc.add(
        Text::new()
            .set("x", x + 55.0)
            .set("y", y + 16.0)
            .set("font-family", "monospace")
            .set("font-size", "12")
            .set("font-weight", "bold")
            .set("fill", "#ff0000")
            .add(TextNode::new(format!(
                "{}/{}",
                vis_data.state.treasure_hitpoints[i], input.treasure_hitpoints[i]
            ))),
    );
    doc
}

fn draw_weapon_usage_gauge(
    mut doc: SVG,
    vis_data: &VisData,
    i: usize,
    x: f64,
    y: f64,
    _cell_width: f64,
    cell_height: f64,
) -> SVG {
    let remaining_uses = vis_data.state.weapon_counts[i];

    if remaining_uses > 0 {
        // Draw weapon usage gauge with squares
        let square_size = 11.0;
        let square_spacing = 2.0;
        let gauge_x = x + 37.0;
        let gauge_y = y + cell_height / 2.0 - square_size / 2.0;

        for j in 0..remaining_uses {
            let square_x = gauge_x + j as f64 * (square_size + square_spacing);
            doc = doc.add(
                Rectangle::new()
                    .set("x", square_x)
                    .set("y", gauge_y)
                    .set("width", square_size)
                    .set("height", square_size)
                    .set("fill", "#228b22")
                    .set("stroke", "#006400")
                    .set("stroke-width", 1.0),
            );
        }
    }

    doc
}

const MAX_HP: f64 = 500.0;

fn draw_hp_gauge(
    mut doc: SVG,
    vis_data: &VisData,
    input: &Input,
    i: usize,
    x: f64,
    y: f64,
    cell_width: f64,
    _cell_height: f64,
    attacker_weapon_index: Option<usize>,
) -> SVG {
    let current_hp = vis_data.state.treasure_hitpoints[i];
    let max_hp = input.treasure_hitpoints[i];

    // Calculate the base bar width (full width when HP = 500)
    let bar_width = cell_width - 40.0;
    let bar_height = 16.0;
    let bar_x = x + cell_width - bar_width - 4.0;
    let bar_y = y + 4.0;

    // Background bar (light gray)
    doc = doc.add(
        Rectangle::new()
            .set("x", bar_x)
            .set("y", bar_y)
            .set("rx", 2.0)
            .set("ry", 2.0)
            .set("width", bar_width)
            .set("height", bar_height)
            .set("fill", "#f0f0f0")
            .set("stroke", "#999")
            .set("stroke-width", 0.5),
    );

    // Remaining HP bar (light blue)
    if current_hp > 0 {
        let remaining_width = bar_width * current_hp as f64 / MAX_HP;
        doc = doc.add(
            Rectangle::new()
                .set("x", bar_x)
                .set("y", bar_y)
                .set("width", remaining_width)
                .set("height", bar_height)
                .set("fill", "#87ceeb")
                .set("stroke-width", 0.0),
        );
    }

    // Attack power bar (red) - shows attack power
    if let Some(attacker_weapon_index) = attacker_weapon_index {
        let attack_power = input.attack_powers[attacker_weapon_index][i];
        let attack_power_width = bar_width * attack_power as f64 / MAX_HP;
        let attack_power_x =
            bar_x + (bar_width * current_hp as f64 / MAX_HP - attack_power_width).max(0.0);
        doc = doc.add(
            Rectangle::new()
                .set("x", attack_power_x)
                .set("y", bar_y + bar_height - 2.0)
                .set("width", attack_power_width)
                .set("height", 2.0)
                .set("fill", "#ff0000")
                .set("stroke-width", 0.0),
        );
    }

    // Damage bar (yellow) - shows damage from last attack
    if let Some(ref op) = vis_data.op {
        let Op::Attack(attack) = op;
        if attack.target_treasure_index == i {
            let damage = if attack.is_hand {
                1
            } else {
                input.attack_powers[attack.weapon_index][i]
            };
            let damage_width = bar_width * damage as f64 / MAX_HP;
            let damage_x = bar_x + (bar_width * current_hp as f64 / MAX_HP);

            doc = doc.add(
                Rectangle::new()
                    .set("x", damage_x)
                    .set("y", bar_y)
                    .set("width", damage_width.min(bar_width - damage_x + bar_x))
                    .set("height", bar_height)
                    .set("fill", "#ffd700")
                    .set("stroke-width", 0.0),
            );
        }
    }

    // HP text
    doc = doc.add(
        Text::new()
            .set("x", x + 55.0)
            .set("y", y + 16.0)
            .set("font-family", "monospace")
            .set("font-size", "12")
            .set("font-weight", "bold")
            .set("fill", "#333")
            .add(TextNode::new(format!("{}/{}", current_hp, max_hp))),
    );

    doc
}