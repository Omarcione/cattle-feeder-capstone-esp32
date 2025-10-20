# cattle-feeder-capstone-esp32
ESP32 firmware for CPE Capstone Cattle Feeder Project
# ESP32 Project Git Workflow

## Overview
This guide explains how to manage branches, push, and pull updates for the ESP32 firmware repository.
Use it to keep your local code in sync with the main branch and collaborate cleanly.

---

## 1. Clone the Repository
```bash
git clone https://github.com/Omarcione/cattle-feeder-capstone-esp32.git
cd cattle-feeder-capstone-esp32
```

---

## 2. Create a New Branch
Always use a branch for feature or fix work.
To create a new branch:
```bash
git checkout -b <descriptive-branch-name>
```
To go to an existing branch:
```bash
git checkout <branch-name>
```

---

## 3. Check Status and Changes
Check current branch.
```bash
git branch
```
View modified files before committing.
```bash
git status
```

---

## 4. Stage and Commit
```bash
git add .
git commit -m \"<description of commit>"
```

---

## 5. Push Branch to Remote
```bash
git push
```

---

## 6. Pull Latest Main Branch
Before merging, sync your branch with main:
```bash
git checkout main
git pull origin main
git checkout <branch-name>
git merge main
```
Resolve merge conflicts if prompted and commit again.

---

## 7. Merge to Main
```bash
git checkout main
git pull origin main
git merge <branch-name>
```
If the merge succeeds:
```bash
git push main
```

---

## Notes
- Always pull `main` before starting new work.
- Keep commit messages short but descriptive.
- Never commit `build/`, `.vscode/`, or `__pycache__/` - add them to `.gitignore`.
