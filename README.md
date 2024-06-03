# Zephyr Playground

This repository contains the source code for the Zephyr playground's App and subsystems.

## Getting Started

The best place to start using this repository is by following Zephyr's Getting Started Guide and getting familiar with the Zephyr Project.

This repository assumes you have installed all of the dependencies listed on the [Getting Started guide by Zephyr](https://docs.zephyrproject.org/3.6.0/develop/getting_started/index.html#getting-started-guide).

Please note this repository is compatible with the [Zephyr Release 3.6.0](https://github.com/zephyrproject-rtos/zephyr/releases/tag/v3.6.0).

### Set Development Environment

```console
mkdir -p ~/zephyrproject
cd ~/zephyrproject
west init -m git@github.com:croxel/zephyr-playground
west update
west patch
```

### Build Sample App

```console
cd ~/zephyrproject/zephyr-playground
source ~/zephyrproject/zephyr/zephyr-env.sh

west build -b croxel_cx1825 app
```


## Git Hooks
In order to follow:

- The *Coding-Style Compliance* convention described in the [SCM - Coding-Style+Compliance](https://croxel.atlassian.net/l/cp/vmS3SPtP)
- The *Commit Structure Requirements* convention described in the [SCM - Commit Structure Requirements](https://croxel.atlassian.net/l/cp/TDfhyUz4)

every single developer should configure the git hooks by doing the following:

```bash
git config core.hooksPath .githooks
```
