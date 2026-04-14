## Release Checklist

Before creating a new release, follow this checklist:

---

### 1. Validate Code Quality

* [ ] All changes are merged into `main`
* [ ] CI is passing (lint + build)
* [ ] No pending Pull Requests for this release

---

### 2. Test the Library

* [ ] All examples compile successfully
* [ ] Tested on target boards (ESP32 / C3 / S2 if applicable)
* [ ] No breaking changes without documentation

---

### 3. Update Version

* [ ] Update version in `library.properties`:

```
version=1.0.0
```

* [ ] Follow Semantic Versioning:

  * Patch: bug fixes (1.0.0 → 1.0.1)
  * Minor: new features (1.0.0 → 1.1.0)
  * Major: breaking changes (1.0.0 → 2.0.0)

---

### 4. Run Arduino Lint

```bash
arduino-lint --compliance strict --library-manager submit
```

* [ ] No errors
* [ ] No important warnings

---

### 5. Update Documentation

* [ ] README updated (if needed)
* [ ] Examples updated (if needed)

---

### 6. Create Git Tag

```bash
git tag 1.0.0
git push origin 1.0.0
```

* [ ] Tag matches `library.properties` version

---

### 7. Create GitHub Release (recommended)

* [ ] Add release title (e.g., `v1.0.0`)
* [ ] Add short description of changes
* [ ] Highlight important updates

---

### Done!

Your release is now ready
