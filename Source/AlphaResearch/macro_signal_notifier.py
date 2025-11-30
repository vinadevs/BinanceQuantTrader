#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************

#!/usr/bin/env python3
"""
macro_notifier.py

Quant macro market news title notifier:
- Scrapes HTML and RSS for titles from a URL
- Deduplicates against existing saved file
- Appends new titles to a CSV/JSON file with timestamp and source
- Optional loop mode with interval for periodic checking

Requirements:
    pip install requests beautifulsoup4 lxml feedparser python-dateutil

How to Use:
1. Save file

Create the file:

macro_notifier.py


Paste the entire code.

2. Install dependencies
pip install requests beautifulsoup4 lxml feedparser python-dateutil

3. Run once
python macro_notifier.py --url https://www.reuters.com/world/ --out news.csv

4. Run in loop (every 5 minutes)
python macro_notifier.py --url https://www.reuters.com/world/ --out news.csv --interval 300
"""

import argparse
import csv
import json
import os
import time
from datetime import datetime
from typing import List, Dict, Set, Optional
import requests
from bs4 import BeautifulSoup
import feedparser
from dateutil import parser as dateparser

def now_iso() -> str:
    return datetime.utcnow().isoformat() + "Z"

def safe_text(s) -> str:
    if s is None:
        return ""
    return " ".join(s.strip().split())

# -------- RSS parsing --------
def parse_rss_titles(url: str) -> List[Dict]:
    d = feedparser.parse(url)
    results = []
    for entry in d.entries:
        title = safe_text(entry.get("title") or "")
        link = entry.get("link", "")
        published = entry.get("published") or entry.get("updated") or ""

        try:
            if published:
                published = dateparser.parse(published).isoformat() + "Z"
        except Exception:
            pass

        if title:
            results.append({"title": title, "link": link, "published": published})
    return results

# -------- HTML parsing --------
def parse_html_titles(url: str, html: str) -> List[Dict]:
    soup = BeautifulSoup(html, "lxml")
    titles = []

    # Try OpenGraph
    og = soup.find("meta", property="og:title")
    if og:
        titles.append({"title": safe_text(og.get("content", "")), "link": url, "published": ""})

    # Try <title>
    if soup.title and soup.title.string:
        titles.append({"title": safe_text(soup.title.string), "link": url, "published": ""})

    # Look for article headers
    for article in soup.find_all("article"):
        for h in article.find_all(["h1", "h2", "h3"]):
            txt = safe_text(h.get_text())
            if txt:
                a = h.find("a")
                link = a["href"] if a and a.get("href") else ""
                titles.append({"title": txt, "link": link, "published": ""})

    # Generic selectors
    selectors = ["h1", "h2", ".headline", ".news-title", ".article-title"]
    for sel in selectors:
        for node in soup.select(sel):
            txt = safe_text(node.get_text())
            if txt:
                a = node.find("a")
                link = a["href"] if a and a.get("href") else ""
                titles.append({"title": txt, "link": link, "published": ""})

    # Deduplicate
    out = []
    seen = set()
    for t in titles:
        key = t["title"].lower()
        if key not in seen:
            seen.add(key)
            out.append(t)
    return out

# -------- File operations --------
def load_existing_titles(out: str) -> Set[str]:
    if not os.path.exists(out):
        return set()

    if out.endswith(".json"):
        try:
            with open(out, "r", encoding="utf-8") as f:
                return {item["title"] for item in json.load(f)}
        except Exception:
            return set()

    if out.endswith(".csv"):
        try:
            with open(out, newline="", encoding="utf-8") as f:
                return {row["title"] for row in csv.DictReader(f)}
        except Exception:
            return set()

    return set()

def save_titles(out: str, rows: List[Dict]):
    if out.endswith(".json"):
        data = []
        if os.path.exists(out):
            try:
                with open(out, "r", encoding="utf-8") as f:
                    data = json.load(f)
            except Exception:
                data = []
        data.extend(rows)
        with open(out, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
        return

    # Default CSV
    exists = os.path.exists(out)
    with open(out, "a", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=["saved_at", "title", "link", "published", "source"])
        if not exists:
            writer.writeheader()
        for r in rows:
            writer.writerow(r)

# -------- Fetching --------
def fetch_url(url: str, timeout: int = 10) -> Optional[str]:
    try:
        r = requests.get(url, headers={"User-Agent": "macro-notifier/1.0"}, timeout=timeout)
        r.raise_for_status()
        return r.text
    except Exception as e:
        print("[ERROR] Failed to fetch:", e)
        return None

def collect_titles(url: str) -> List[Dict]:
    # Try RSS first
    feed = feedparser.parse(url)
    if feed.bozo == 0 and feed.entries:
        out = parse_rss_titles(url)
        for t in out:
            t["source"] = url
            t["saved_at"] = now_iso()
        return out

    html = fetch_url(url)
    if not html:
        return []

    out = parse_html_titles(url, html)
    for t in out:
        t["source"] = url
        t["saved_at"] = now_iso()
    return out

# -------- Main logic --------
def run_once(url: str, out: str):
    print(f"[{now_iso()}] Checking {url} ...")
    titles = collect_titles(url)

    if not titles:
        print("No titles found.")
        return

    existing = load_existing_titles(out)
    new_items = [t for t in titles if t["title"] not in existing]

    if not new_items:
        print("No new titles.")
        return

    print(f"Found {len(new_items)} new titles:")
    for t in new_items:
        print(" -", t["title"])

    save_titles(out, new_items)
    print("Saved to", out)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--url", "-u", required=True, help="Website or RSS feed URL")
    parser.add_argument("--out", "-o", default="headlines.csv", help="Output file (csv or json)")
    parser.add_argument("--interval", type=int, default=0, help="Loop interval in seconds (0 = run once)")
    args = parser.parse_args()

    if args.interval <= 0:
        run_once(args.url, args.out)
        return

    print(f"[LOOP MODE] Fetch every {args.interval} seconds ...")
    while True:
        run_once(args.url, args.out)
        time.sleep(args.interval)

if __name__ == "__main__":
    main()
