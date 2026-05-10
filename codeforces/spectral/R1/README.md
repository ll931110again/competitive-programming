# Spectral Cup 2026 Round 1

Codeforces Round 1094 (Div. 1 + Div. 2): **<https://codeforces.com/contest/2222>**

## Solutions

Sources are named after each problem title (see filenames).

## Statements

Offline copies (HTML from the statement tab) live under [`statements/`](statements/). Generate or refresh them with:

```bash
python3 fetch_statements.py
```

The script reads **`mirror.codeforces.com`** because automated `curl` to **`codeforces.com`** often hits a Cloudflare challenge; links inside each file still point at the official contest URLs.

### Saved “Problems” page from your browser

If you use **Save Page As…** on `https://codeforces.com/contest/2222/problems`, you can extract every `problem-statement` locally (no network):

```bash
python3 parse_saved_cf_problems_html.py "/path/to/Problems - Codeforces.html"
```

Outputs under **`statements_from_browser/`** (same title stems as **`statements/`**, plus **F–H**). Those files include full rendered MathJax markup from the save, so they are larger than the mirror HTML.
