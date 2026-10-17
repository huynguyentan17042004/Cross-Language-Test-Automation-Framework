# Dragon Warriors Release Package

This package contains a platform-specific console executable built from the preserved C++
SUT snapshot, sample configuration files, a machine-readable build manifest,
and the JUnit report produced before packaging.

Run from the extracted package directory so the executable can resolve the
bundled `config/` paths:

```bash
./dragon-warriors
```

The binary is platform-specific. For example, a Linux artifact is not expected
to run on macOS or Windows. Verify the adjacent `.sha256` file before
distribution.
