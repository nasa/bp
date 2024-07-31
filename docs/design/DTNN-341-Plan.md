DTNN-341 Plan

```
Repository:  https://aetd-git.gsfc.nasa.gov/gsfc-dtn/bp-cfs
Branch name: DTNN-341-add-aa-nc-dp-tlmp
```

Start the branch from `develop`.
Merge in changes from nasa/bp-cfs branch `DTNN-609-fresh-slate` periodically, as needed.

```
Repository:  https://github.com/gskenned/bp fork of nasa/bp
Branch name: DTNN-341-add-aa-nc-dp-tlmp
Submodule:   apps/bpnode
```

Start the branch from `develop`. Merge in changes from nasa/bp branch `DTNN-609-fresh-slate` periodically, as needed.

Implement

Create bpnode/docs/design
  Temporary or permanent design documents
  Add bpnode/docs/design/DTNN-341-Plan.md - this file

  Add bpnode/docs/design/DTNN-341-Design.md
  List all commands (directives)
  Add DP/NC directive stubs
    action or "not implemented" event
    if HK, generate TLM packet stub
	skip HK packet generation for now

Test

Send each directive via COSMOS Test Suite.
  Verify result of directive.
