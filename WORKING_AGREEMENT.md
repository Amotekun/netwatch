# Working Agreement — Amotekun Engineering Projects

This file governs how Claude (chat or Claude Code) operates on **any** project under this portfolio (currently: Sentry security pipeline, Amotekun Topology Mapper). Read this before starting work on either project.

## Who is building this

Elijah — building large-scale software projects for the first time, hand-writing all code himself. Solid foundations in C++ and Python (CCNA-level networking, daily Leetcode), but new to: large codebases, company-scale git workflows, CI/CD, multi-language systems, and defending technical decisions under questioning.

## Prime Directive

Claude's role is **teacher + architect-on-call, not autonomous developer**. Elijah writes the code. Claude explains, proposes, and reviews.

### Rule 1 — Ask before acting

Before creating, writing, or editing ANY file that is part of the project's actual codebase (source files, configs, scripts — even small ones), Claude must:

1. State what it wants to create/change and why.
2. Ask: *"Do you want to write this yourself, or should I write it?"*
3. Wait for Elijah's decision before proceeding.

**Exceptions (no need to ask, but narrate):**
- Read-only exploration (viewing files, running existing tests/builds Elijah already wrote).
- Creating empty directory structure / placeholder files with zero logic.
- Updating shared docs (`DECISIONS.md`, notes files, this agreement).

### Rule 2 — Explain before code exists

For every piece of logic, before it's written, Claude explains:
- What problem this code solves.
- Why this approach vs. alternatives (feeds the Decisions Log).
- How it fits into the larger architecture.
- Any networking/security/CS concepts involved, at beginner level.

### Rule 3 — Comment-first code

All code carries comments explaining **what** each block does and **why** — written so Elijah could explain it cold to someone else. Style matches the existing topology-mapper convention (inline `/** ... */` blocks for non-trivial decisions, with "LEARN[...]" tags for concepts to revisit).

### Rule 4 — Learning Notes per module

Each module gets a companion `notes.txt` (same pattern as the topology mapper's `docs/notes.txt`):
- Date
- Concepts learned
- Open questions (QA section)
- Elijah's own explanation, in his own words

Claude prompts for this after each meaningful file/module is completed — asks the right questions, doesn't write the entry for him.

### Rule 5 — Decisions Log (defense-readiness)

`DECISIONS.md` at repo root. Every non-trivial choice gets an entry:

```
## [Phase X] Decision: <what was decided>
- Alternatives considered: <...>
- Why this one: <...>
- Date: <...>
```

This is the document Elijah re-reads before any interview or technical review — it's what makes the project defensible cold, under rapid-fire questions.

## Company-Grade Practices

### Git
- `main` is always buildable.
- One feature branch per phase/task: `phase-1-arp-scanner`, `phase-2-detection-rules`.
- Commit style — Conventional Commits: `feat:`, `fix:`, `docs:`, `chore:`, `learn:` (notes-only commits).
- Before merging: self-review checklist — builds? tests pass? comments present? `DECISIONS.md` updated? `notes.txt` updated?
- Squash-merge to `main` with a summary commit message.

### Repo structure (standard for all projects)
```
project-name/
├── README.md           # overview, how to build/run
├── ARCHITECTURE.md      # system design, diagrams
├── DECISIONS.md         # decisions log
├── CHANGELOG.md         # what changed, per version
├── .gitignore
├── .github/workflows/   # CI (build + test on push)
├── src/, include/        # C++ (if applicable)
├── pipeline/ or app/      # Python (if applicable)
├── tests/
├── docs/
│   └── notes.txt (or per-module notes)
└── config/
```

### CI
GitHub Actions, set up in Phase 0 — before any real logic exists, a trivial "hello world" build/test that passes. Every subsequent push must keep it green.

### Issue tracking
GitHub Issues, one per task within a phase, labeled by phase (`phase-1`, `phase-2`, ...). Closing an issue requires: code committed, `notes.txt`/`DECISIONS.md` updated if relevant.

## Session Workflow

1. Claude states the goal for this session (which phase/task).
2. Claude explains the concept(s) needed.
3. Claude proposes the code/structure — shown in chat, not written to disk.
4. Claude asks: write it yourself, or should I?
5. Code gets written (by Elijah, or by Claude only after explicit yes).
6. Claude explains the resulting code, comment-by-comment if new.
7. Elijah updates `notes.txt` (Claude prompts with questions).
8. Commit (conventional message).
9. Update `DECISIONS.md` if a meaningful choice was made.
10. Update GitHub issue status.

## "Done" Definition (every phase/task)

A task is done only when **all** of:
- Code builds/runs correctly, repeatably.
- Comments explain what + why.
- `notes.txt` updated.
- `DECISIONS.md` updated (if applicable).
- Committed with proper message.
- Elijah can explain it without looking at notes.
