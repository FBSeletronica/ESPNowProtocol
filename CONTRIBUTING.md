# Contributing

Contributions are welcome! This project follows a simple and reliable workflow to ensure code quality and consistency.

---

## Workflow Overview

1. Fork the repository
2. Create a new branch from `main`
3. Make your changes
4. Open a Pull Request

---

## Development Setup

1. Clone your fork:

```
git clone https://github.com/FBSeletronica/ESPNowProtocol.git
cd ESPNowProtocol
```

2. Make sure you have:

* Arduino CLI installed
* ESP32 core installed

---

## Testing

Before submitting a Pull Request:

* Ensure all examples compile
* Run Arduino Lint:

```
arduino-lint --compliance strict --library-manager submit
```

---

## CI Behavior

This project uses GitHub Actions for validation:

* **Lint** runs on every commit
* **Build** runs:

  * On all Pull Requests
  * On commits containing `[build]` or `[ci]`

To force a full build:

```
git commit -m "fix: adjust protocol [build]"
```

---

## Commit Convention

This project uses **Conventional Commits**:

```
feat: add new feature
fix: resolve bug
docs: update documentation
chore: maintenance tasks
```

Only `feat` and `fix` should represent changes that may affect a release.

---

## Pull Request Guidelines

* Keep PRs small and focused
* Do not include unrelated changes
* Ensure CI passes before requesting review
* Update documentation if needed

---

## Releases

Releases are managed by the project maintainer.

Contributors do **not** need to create tags or releases.

When a new version is ready, the maintainer will:

1. Update the version in `library.properties`
2. Run Arduino Lint:

```
arduino-lint --compliance strict --library-manager submit
```

3. Create a Git tag:

```
git tag 1.0.0
git push origin 1.0.0
```

4. Create a release on GitHub (optional but recommended)

---

## Questions

If you have any questions, feel free to open an issue.

---

Thank you for contributing!
