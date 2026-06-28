# Abdullah Alshenaifi — Portfolio Website

Personal portfolio for Abdullah Alshenaifi, Electronics Engineer.  
Live at: **[aashenaifi.com](https://aashenaifi.com)**

---

## Files

| File | Description |
|---|---|
| `index.html` | The entire website — self-contained, no build step |
| `content.json` | Runtime content (projects, experience, skills, socials). Edited via dashboard |
| `logo.glb` | Interactive 3D logo model (hero section, drag to rotate) |
| `logo-anim.svg` | Animated circuit-trace logo (boot sequence + 3D fallback) |
| `CV-Abdullah-Alshenaifi-EN.pdf` | English CV (served via the CV section buttons) |

**Total: 5 files.** No node_modules. No build step. Drop and serve.

---

## How it works

`index.html` loads `content.json` at runtime via `fetch()`. If the JSON is missing the site still works — it falls back to built-in defaults. The 3D model uses the [`<model-viewer>`](https://modelviewer.dev/) web component loaded from jsDelivr CDN.

To update content (projects, experience, links, etc.) edit via the **private dashboard** — it publishes changes to `content.json` in this repo through the GitHub API.

---

## Sections

- **Boot screen** — animated logo + power-on sequence (skippable)  
- **Hero** — interactive 3D logo, oscilloscope (Sine / Square / PWM / Triangle / Sawtooth / AM / FM)  
- **About** — bio, skills  
- **Projects** — real projects from CV (PCB design, IoT, embedded systems)  
- **AI Tools** — demo tools built on Netlify (marked in-testing)  
- **Services** — request form (Formspree or mailto fallback)  
- **CV** — View + Download buttons (English / Arabic)  
- **Contact** — links and socials  
- **Arabic / English** — full bilingual support, Cairo font for both

---

## Hosting

Served as a static site on **GitHub Pages** from the repo root.  
No CI, no build pipeline — GitHub Pages serves `index.html` directly.

---

## Content updates

Content is managed through a **separate private dashboard** (hosted on Netlify).  
The dashboard authenticates with a fine-grained GitHub token stored only in your browser — never committed here.  
All data in `content.json` is public portfolio information; it is safe in a public repo.

---

## Local preview

```bash
python -m http.server 5500
