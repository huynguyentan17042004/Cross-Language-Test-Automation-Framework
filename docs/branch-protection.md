# Branch Protection Checklist

After the combined branch is pushed and its workflow is green, protect
`python-driven-version` in GitHub repository settings:

1. Open **Settings → Branches → Add branch protection rule**.
2. Branch name pattern: `python-driven-version`.
3. Enable **Require a pull request before merging**.
4. Enable **Require status checks to pass before merging**.
5. Select `Required CI quality gate`.
6. Enable **Require branches to be up to date before merging** if the repository
   workflow permits the extra reruns.
7. Do not require the scheduled memory job; it does not run on every PR.

Verification requires GitHub-side evidence: open a controlled PR with a failing
test, confirm merge is blocked, then fix it and confirm the gate becomes green.
Repository settings cannot be proven by local source files alone.
