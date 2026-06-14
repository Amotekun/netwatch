# Security Pipeline ("Sentry") — Project Specification

> Governed by `WORKING_AGREEMENT.md` — read that first. This is the **active** project.

## Identity
- Codename: **Sentry**
- Part of: Amotekun engineering portfolio
- Status: **ACTIVE**

## Problem Statement

Raw network traffic is just noise — packets flow by and disappear unless something captures, structures, and stores them. This pipeline turns that noise into a continuous, queryable record: every event is captured, normalized, stored, and checked against detection rules in real time, with alerts surfaced on a dashboard and historical patterns available for later investigation. At a small scale, it's exactly the problem SIEM platforms (Splunk, Wazuh) solve for security teams — converting raw telemetry into visibility and actionable alerts.

## Architecture — Full Stack (target end state)

1. **Capture — C++ / libpcap** — extends the topology mapper's networking foundations. Captures raw packets, extracts structured fields (IPs, ports, protocol, timestamps), emits events continuously.
2. **Streaming bus — Kafka** — capture publishes events to a topic. Decouples capture from processing — capture never stops even if downstream is slow/down. (Phase 3+)
3. **Processing — Python (Kafka consumer)** — reads events, normalizes/parses, applies detection rules, writes to DB + flags alerts. (Phase 2+)
4. **Storage — SQLite → Postgres** — raw events + alerts table. SQLite for V1; Postgres once Kafka is involved.
5. **Batch orchestration — Airflow** — jobs on top of stored data: daily summary reports, retention/cleanup. (Phase 4)
6. **Dashboard — Grafana** (reading from Postgres) — live view of traffic + alerts. (Phase 5)
7. **Docker Compose** — wraps Kafka, Postgres, Python services, Grafana into one runnable stack. (Phase 6)

## Repo Structure

```
sentry-pipeline/
├── README.md
├── ARCHITECTURE.md
├── DECISIONS.md
├── DEFENSE.md            # Q&A prep, see below
├── CHANGELOG.md
├── .gitignore
├── .github/workflows/ci.yml
├── capture/               # C++ component
│   ├── src/
│   ├── include/
│   ├── Makefile
│   └── notes.txt
├── pipeline/               # Python component
│   ├── ingest/
│   ├── detect/
│   ├── db/
│   └── notes.txt
├── dashboards/             # Grafana configs (Phase 5)
├── orchestration/          # Airflow DAGs (Phase 4)
├── docker/                 # Compose files (Phase 3+)
├── tests/
└── docs/
```

## Phases

### Phase 0 — Setup (Foundation)
**Goal:** repo exists, CI is green, structure in place.

Tasks:
- Init git repo, `.gitignore` (C++ + Python)
- Create repo structure (empty dirs/placeholders)
- `README.md` skeleton (problem statement, architecture diagram placeholder)
- `DECISIONS.md`, `CHANGELOG.md`, `DEFENSE.md` initialized
- GitHub Actions: build a C++ "hello world" + run one trivial Python test

**Done when:** `git log` shows initial commits; CI passes on a no-op build/test.

---

### Phase 1 — V1: Minimal End-to-End Slice
**Goal:** prove the pipe works. Nothing fancy.

Tasks:
- C++: `libpcap` capture of ~50 packets, extract src/dst IP, ports, protocol, timestamp → write as JSON Lines to a file
- Python: read the JSON Lines file, insert rows into SQLite, print row count

**Concepts:** `libpcap` basics, JSON Lines as an interchange format, SQLite schema design for event data

**Done when:** running both produces correct, repeatable row counts; comments + `notes.txt` done; `DECISIONS.md` entries for "why JSON Lines" and "why SQLite for V1"

**Hard rule:** no Kafka, Airflow, detection rules, or dashboard until this is fully done.

---

### Phase 2 — Detection Layer
**Goal:** turn stored events into alerts.

Tasks:
- Design `alerts` table schema
- Implement a small rules engine in Python (hand-rolled first; introduce Sigma rule format once comfortable)
- At least 2 working detection rules against test data (e.g., port-scan pattern, unusual destination)

**Concepts:** Sigma rule format, detection rule design tradeoffs (false positives vs. false negatives)

**Done when:** rules fire correctly against known test data; explained; `DECISIONS.md` entry on rule design choices

---

### Phase 3 — Streaming (Kafka)
**Goal:** decouple capture from processing.

Tasks:
- Introduce Kafka (via Docker Compose) between C++ capture and Python processing
- C++ becomes a Kafka producer; Python becomes a Kafka consumer
- Postgres replaces SQLite (handles concurrent writes)

**Concepts:** pub/sub messaging, why decoupling matters, Postgres vs. SQLite tradeoffs

**Done when:** same data flow as Phase 1 but through Kafka; Docker Compose brings up Kafka + Postgres; `DECISIONS.md` entry on "why Kafka now, why not earlier"

---

### Phase 4 — Orchestration (Airflow)
**Goal:** scheduled batch jobs on top of stored data.

Tasks:
- Daily summary report DAG
- Retention/cleanup DAG

**Concepts:** DAGs, task dependencies, why cron isn't enough once jobs depend on each other

**Done when:** Airflow runs both DAGs on schedule against real data

---

### Phase 5 — Dashboard (Grafana)
**Goal:** visualize events + alerts.

Tasks:
- Grafana connected to Postgres
- Panels: event volume over time, alerts by severity, top talkers

**Done when:** dashboard shows live data updating as the pipeline runs

---

### Phase 6 — Full Stack Integration
**Goal:** one command runs everything.

Tasks:
- Docker Compose ties together capture, Kafka, Postgres, Python services, Airflow, Grafana
- Final `README.md` polish, `ARCHITECTURE.md` diagram

**Done when:** `docker compose up` (plus running the C++ capture binary) runs the entire stack end-to-end

---

## DEFENSE.md — Format

A running Q&A log. Add entries as questions come up (from Claude, from real interviews, from self-review):

```
Q: Why not just use Wazuh/Splunk?
A: <Elijah's answer>

Q: How does this scale beyond a single host?
A: <Elijah's answer>

Q: What's the false positive rate on your detection rules?
A: <Elijah's answer>
```

This file is reviewed before any interview or technical conversation about the project.
