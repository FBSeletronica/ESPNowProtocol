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


--- 

# Versioning and Releases

This project follows a simple and manual release process compatible with the Arduino Library Manager.

---

## How Releases Work

Once the library is accepted into the Arduino Library Manager:

* New versions are detected automatically
* No additional submission is required
* The system checks for new Git tags periodically

---

## How to Publish a New Version

Follow these steps to release a new version of the library:

---

### 1. Make your changes

Develop and test your changes as usual.

---

### 2. Update the version

Edit the `library.properties` file:

```ini id="nqmh9q"
version=1.0.1
```

> The version must be higher than the previous one.

---

### 3. Validate the library

Run Arduino Lint:

```bash id="gq4s6p"
arduino-lint --compliance strict --library-manager submit
```

Make sure there are no errors.

---

### 4. Commit the version update

```bash id="x3d67g"
git add library.properties
git commit -m "chore: bump version to 1.0.1"
```

---

### 5. Create a Git tag

```bash id="0f02sf"
git tag 1.0.1
git push origin main
git push origin 1.0.1
```

---

### 6. Done!

The Arduino Library Manager will automatically:

* Detect the new tag
* Validate the release
* Add it to the index

This usually takes a few hours.

---

## Important Rules

* The version in `library.properties` must match the Git tag
* Versions must always increase (no duplicates)
* The library must remain compliant with Arduino standards
* Invalid releases will be ignored by the indexer

---

## Check Release Status

You can monitor the indexing process here:

```
http://downloads.arduino.cc/libraries/logs/github.com/FBSeletronica/ESPNowProtocol/
```

---

## Versioning Strategy

This project follows Semantic Versioning:

* **Patch** (`1.0.0 → 1.0.1`): bug fixes
* **Minor** (`1.0.0 → 1.1.0`): new features
* **Major** (`1.0.0 → 2.0.0`): breaking changes

---

## Summary

```
Update version → Commit → Tag → Push → Done
```

No manual submission is needed after the first release.

---

Happy coding!
