# Release Artifact Process

LAB 10 packages the console application; it does not deploy a web service.

## Manual build

1. Generate test evidence:

   ```bash
   python -m pytest --layer all --junitxml=reports/release-tests.xml
   ```

2. Build the versioned package:

   ```bash
   python scripts/build_release.py \
     --version 1.0.0 \
     --test-report reports/release-tests.xml \
     --output-dir dist
   ```

3. Verify the checksum from inside `dist/`:

   ```bash
   shasum -a 256 -c dragon-warriors-*.sha256
   ```

The package contains the executable, four sample configs, README, build
manifest, and JUnit evidence. Archive names include version, OS, and CPU
architecture. GitHub Actions can run the same process through
`workflow_dispatch` or a `v*` tag and retains the artifact for 30 days.

Creating a permanent GitHub Release remains an explicit optional approval step;
the workflow does not publish one automatically.
