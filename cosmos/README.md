# OpenC3 COSMOS Plugin

See the [OpenC3](https://openc3.com) documentation for all things OpenC3.

## Building non-tool / widget plugins

1. <Path to COSMOS installation>/openc3.sh cli rake build VERSION=X.Y.Z (or openc3.bat for Windows)
   - VERSION is required
   - gem file will be built locally

## Building tool / widget plugins using a local Ruby/Node/Yarn/Rake Environment

1. yarn
2. rake build VERSION=1.0.0

## Building tool / widget plugins using Docker and the openc3-node container

If you don’t have a local node environment, you can use our openc3-node container to build custom tools and custom widgets

Mac / Linux:

```
docker run -it -v `pwd`:/openc3/local:z -w /openc3/local docker.io/openc3inc/openc3-node sh
```

Windows:

```
docker run -it -v %cd%:/openc3/local -w /openc3/local docker.io/openc3inc/openc3-node sh
```

1. yarn
2. rake build VERSION=1.0.0

## Installing into OpenC3 COSMOS

1. Go to the OpenC3 Admin Tool, Plugins Tab
2. Click the paperclip icon and choose your plugin.gem file
3. Fill out plugin parameters
4. Click Install

## DTN Tool Integration

The DTN Tools Suite require COSMOS version 5.12 and later, due to a Python package dependency. You can install DTN Tools wheel file (.whl) through the COSMOS Administrator Console via the Packages tab (Don't forget to click Upload!). When configuring test environments, any host ports or folders accessed by the DTN test scripts must be explicitly defined in the COSMOS compose.yaml file to ensure proper container communication. After integration you can use DTN Tools in scripts and procedures within the COSMOS Script Runner tab. See the [BPNode User Guide](https://github.com/nasa/bp/tree/main/docs) for more details.