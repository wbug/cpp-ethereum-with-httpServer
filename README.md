# cpp-ethereum with httpServer - Ethereum C++ client


23 Nov 2017, cpp-ethereum 把 httserver 给删除了， 但很多时候我们需要它：
- 用 C++ 写的 HttpServer 相比用其它语言写的 httpProxy 来比更轻便的
- 对于cpp-ethereum 新手们，使用基于 http的 jsonRpc2.0与cpp-ethereum交互是很多人感觉很自然的事。
- httpServer 默认是不启动的， 同时我们也可以通过在cmake/ProjectJsonRpcCpp.cmake 里设置 -DHTTP_SERVER=OFF 在编译的把httpServer组关了

所以在这里我把 httpServer 给带了回来，同时定期与 https://github.com/ethereum/cpp-ethereum  进行同步

要使用httpServer，在启动时候加参数： -j ， 如果需要指定端口加参数： --json-rpc-port 8545


 23 Nov 2017 , cpp-ethereum drop http server. but sometime i think we need it ：
 
- HttpServer in C++ is exceptionally portable other than using httpProxy in other language.
- It is easy to startup for beginner if we have HttpServer in cpp-ethereum(http looks friendly).
- HttpServer is not started by default when we run cpp-ethererm . and we can tun off the httpServer by set  -DHTTP_SERVER=OFF in cmake/ProjectJsonRpcCpp.cmake.


so we  bring back HTTP server to cpp-ethereum Node. and try synchronize with https://github.com/ethereum/cpp-ethereum 

when use httpServer, add args: -j  , you can specific a port for http by args:  --json-rpc-port 8545








## Contact


## Building from source

### Get the source code

Git and GitHub is used to maintain the source code. Clone the repository by:

```shell
git clone --recursive https://github.com/wbug/cpp-ethereum-with-httpServer.git
cd cpp-ethereum-with-httpServer
```

The `--recursive` option is important. It orders git to clone additional 
submodules which are required to build the project.
If you missed it you can correct your mistake with command 
`git submodule update --init`.

### Install CMake

CMake is used to control the build configuration of the project. Quite recent 
version of CMake is required 
(at the time of writing [3.4.3 is the minimum](CMakeLists.txt#L25)).
We recommend installing CMake by downloading and unpacking the binary 
distribution  of the latest version available on the 
[**CMake download page**](https://cmake.org/download/).

The CMake package available in your operating system can also be installed
and used if it meets the minimum version requirement.

> **Alternative method**
>
> The repository contains the
[scripts/install_cmake.sh](scripts/install_cmake.sh) script that downloads 
> a fixed version of CMake and unpacks it to the given directory prefix. 
> Example usage: `scripts/install_cmake.sh --prefix /usr/local`.

### Install dependencies (Linux, macOS)

The following *libraries* are required to be installed in the system in their
development variant:

- leveldb

They usually can be installed using system-specific package manager.
Examples for some systems:

Operating system | Installation command
---------------- | --------------------
Debian-based     | `sudo apt-get install libleveldb-dev`
RedHat-based     | `dnf install leveldb-devel`
macOS            | `brew install leveldb`


We also support a "one-button" shell script 
[scripts/install_deps.sh](scripts/install_deps.sh)
which attempts to aggregate dependencies installation instructions for Unix-like
operating systems. It identifies your distro and installs the external packages.
Supporting the script is non-trivial task so please [inform us](#contact)
if it does not work for your use-case.

### Install dependencies (Windows)

We provide prebuilt dependencies required to build the project. Download them
with the [scripts/install_deps.bat](scripts/install_deps.bat) script.

```shell
scripts/install_deps.bat
```

### Build

Configure the project build with the following command. It will create the 
`build` directory with the configuration.

```shell
mkdir build; cd build  # Create a build directory.
cmake ..               # Configure the project.
cmake --build .        # Build all default targets.
```

On **Windows** Visual Studio 2015 is required. You should generate Visual Studio 
solution file (.sln) for 64-bit architecture by adding 
`-G "Visual Studio 14 2015 Win64"` argument to the CMake configure command.
After configuration is completed the `cpp-ethereum.sln` can be found in the
`build` directory.

```shell
cmake .. -G "Visual Studio 14 2015 Win64"
```

## License

All contributions are made under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.en.html). See [LICENSE](LICENSE).
