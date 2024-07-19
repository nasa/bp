## bp Example Scripts

Define `CFS_HOME` in `.bashrc` with `export CFS_HOME=~/cfs-home`

`CFS_HOME` can be any folder for which the user has rwx permissions.

Copy the example scripts in this directory to `$CFS_HOME`.

Verify that the environment variables in `$CFS_HOME/cfs-env-vars` are set to the correct paths.

Note that only the path identified by `$CFS_HOME` has to exist prior to running the scripts. The scripts use the other paths to clone, build, and run bplib with cFS.

The default paths are:

```
export CFS_HOME=~/cfs-home                # The build/test folder
export CFS_REPO=$CFS_HOME/cfs-bundle      # A clone of https://github.com/nasa/cFS
export BP_SOURCE=$CFS_REPO/apps/bp        # The bp folder within the cFS clone
export BPLIB_SOURCE=$CFS_REPO/libs/bplib  # The bplib folder within the cFS clone
```

Run the scripts in the following order:

Review the `full-bp-test`, `prep-bp-build` and `clean-bp` scripts to understand that the `$CFS_REPO` clone gets deleted as the first step in `full-bp-test`. Creating `$CFS_REPO` is part of the test.

```sh
   cd $CFS_HOME
   source cfs-env-vars
   # Clean/onfigure/build/test
   ./full-bp-test   # Runs clean-bp, install-toolchain, build-bp, and bp-test
```

