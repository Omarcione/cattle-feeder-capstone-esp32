# cattle-feeder-capstone-esp32
ESP32 firmware for CPE Capstone Cattle Feeder Project
# ESP32 Project Git Workflow

## Overview
This guide explains how to manage branches, push, and pull updates for the ESP32 firmware repository.  
Use it to keep your local code in sync with the main branch and collaborate cleanly.

---

## 1. Clone the Repository
```bash
git clone https://github.com/<your-username>/<repo-name>.git
cd <repo-name>
2. Create a New Branch
Always create a branch for feature or fix work.

bash
Copy code
git checkout -b feature/<name>
Example:

bash
Copy code
git checkout -b feature/add-hcsr04-support
3. Check Status and Changes
View modified files before committing.

bash
Copy code
git status
4. Stage and Commit
bash
Copy code
git add .
git commit -m "Add HC-SR04 distance sensor integration"
5. Push Branch to Remote
bash
Copy code
git push origin feature/<name>
6. Pull Latest Main Branch
Before merging, sync your branch with main:

bash
Copy code
git checkout main
git pull origin main
git checkout feature/<name>
git merge main
Resolve merge conflicts if prompted.

7. Open a Pull Request
Go to your GitHub repo in a browser.

Click Compare & Pull Request.

Add a clear title and description.

Submit for review or merge when approved.

8. Update Local Branches
After merges, keep local branches current:

bash
Copy code
git checkout main
git pull origin main
git branch -d feature/<name>
9. Common Commands
Task	Command
View all branches	git branch -a
Delete local branch	git branch -d <name>
Delete remote branch	git push origin --delete <name>
Revert last commit	git revert HEAD

Notes
Always pull main before starting new work.

Keep commits small and descriptive.

Never commit build/, .vscode/, or __pycache__/—add them to .gitignore.

Copy code






