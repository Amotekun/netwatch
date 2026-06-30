# VISION.md — The Sentry Project: Where This Could Go

> Written June 2026. This is a living document — revisit it when motivation dips, when the code is hard, and when the project ships.

---

## What Sentry Is Today

A packet capture pipeline. C++ reads raw traffic off a network interface, extracts structured fields, writes them to disk. Python reads those fields, stores them in a database. Small. Focused. Real.

That's Phase 1. It works. You built it.

---

## What The Market Looks Like

The existing tools:

| Tool | What it does | What it doesn't do |
|------|-------------|-------------------|
| Wireshark | Captures and displays everything | Explains nothing. Overwhelming to beginners. |
| Snort / Suricata | Real-time intrusion detection | Requires expert configuration. Black box to newcomers. |
| Wazuh / Elastic SIEM | Enterprise SIEM, log aggregation, alerting | Complex deployment. Not educational. Not approachable. |
| Zeek | Deep protocol analysis | Academic. Hard to operationalize. |

None of them explain themselves. None of them were built to teach. They were built by experts, for experts.

**That is the gap.**

---

## The Real Opportunity

Sentry's unfair advantage — if pursued — is not raw performance or enterprise features. It is **legibility**.

A pipeline that captures a packet and says:

> "This is a DNS query from your machine to 8.8.8.8 asking for the IP address of google.com. DNS runs over UDP (protocol 17) on port 53. Your machine does this hundreds of times a day — once for every domain name it needs to resolve."

No tool does this today at the network layer for a general audience. Security teams don't need it. But:

- University networking courses do.
- Bootcamp graduates trying to break into security do.
- Self-taught engineers like you, right now, do.
- Corporate security awareness training programs do.
- Home lab enthusiasts do.

This is a real, underserved audience.

---

## The Technical Trajectory (Honest Version)

Phase 1 through 6 as specced gets you a working SIEM-lite. That alone is a formidable portfolio project. But the longer arc looks like this:

**Year 1 — Foundation (where you are now):**
- C++ capture + Python processing + SQLite + Kafka + Grafana dashboard
- Detection rules engine
- One machine, one interface, works reliably

**Year 2 — The Explanation Layer:**
- Protocol classification engine — not just "this is TCP" but "this is a TLS 1.3 handshake to Cloudflare's infrastructure, likely your browser loading a webpage"
- Human-readable alert narratives generated alongside raw data
- Web interface (not just Grafana) with contextual explanations per packet type

**Year 3 — Scale and Breadth:**
- Multi-host capture (agents on multiple machines reporting to a central pipeline)
- Distributed Kafka topology
- Postgres at scale, time-series optimization
- Protocol coverage: DNS, HTTP/S, DHCP, ARP, ICMP, QUIC

**Year 4+ — The Intersection You're Imagining:**
- At this point the architecture is a general-purpose telemetry pipeline. The source of events doesn't have to be a network interface — it can be anything that produces structured data.
- IoT sensor data from embedded devices
- SDR (Software Defined Radio) — capturing RF signals, decoding ADS-B (aircraft transponders), AIS (maritime vessel tracking)
- Integration with public feeds: threat intelligence, BGP routing tables, OSINT
- The "civilian aircraft communicating with towers" vision becomes real here — ADS-B data is publicly broadcast and decodable with a $25 USB dongle and the right software

**The AI Intersection:**
- Anomaly detection models trained on your own captured baseline traffic
- LLM-generated explanations of alert patterns ("this looks like a port scan — here's why, here's what to do")
- The pipeline becomes training data for security ML models

---

## What Would Make This Unstoppable

Three things, in order:

1. **You finishing Phase 1.** Not imagining Phase 7. Finishing Phase 1. The vision means nothing if the foundation leaks.

2. **Open sourcing it at the right moment.** When it can explain itself — when a non-engineer can run it and understand what they're seeing — that's the moment to put it in front of people. Not before.

3. **Documentation that matches the code.** The explanation layer isn't just a UI feature. It's the entire thesis of the project. Every protocol you add needs a companion explanation written in plain English. That starts now, in `notes.txt`, in `DECISIONS.md`, in your own words.

---

## Competitors Who Could Move Into This Space

Be honest about the threat:

- **Zeek** could add an explanation layer. It hasn't.
- **Elastic** could build a beginner mode. It won't — their market is enterprise.
- **Wireshark** could add contextual tooltips. It's a 25-year-old open source project — it moves slowly.
- **A well-funded startup** could build this in 18 months with a team of 10. This is the real threat. The answer is to move, ship, and establish presence before they notice the gap.

---

## The Manifesto (Your Words, Kept Verbatim)

> "motivation will come and go, but discipline and knowing where i am going beats anything even the behemoth of SELF"

That's the whole thing. Read it when the code is hard. Read it when `pcap_loop` returns an error you don't understand. Read it when you've been staring at a segfault for two hours.

The pipeline doesn't care about motivation. It compiles or it doesn't.

---

## What To Do Right Now

Close this file. Go extract source and destination ports from the TCP/UDP header. That's the next 30 minutes. The vision is real — but it's only reachable one commit at a time.

*— Written with Claude, June 2026*
