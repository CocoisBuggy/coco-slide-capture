{
  description = "Python Film Scanning Utility";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        llvmPackages = pkgs.llvmPackages_latest;
      in
      {
        # Define a reproducible development shell.
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            # Build tools
            cmake
            ninja
            pkg-config
            # C++ toolchain
            llvmPackages.clang-tools
            llvmPackages.clang
            llvmPackages.libcxx
            llvmPackages.lldb

            boost
            fmt
            gtk4
            pkg-config
            libusb1

            # Other development utilities
            gdb
            valgrind
          ];

          shellHook = ''
            echo "✨ C++ development shell with Clang"

            # The following sets up CMake to find the compiler and other tools
            export CC=${llvmPackages.clang}/bin/clang
            export CXX=${llvmPackages.clang}/bin/clang++

            # This is important for finding shared libraries at runtime
            export LD_LIBRARY_PATH=${
              pkgs.lib.makeLibraryPath (
                with pkgs;
                [
                  llvmPackages.libcxx
                  boost
                  fmt
                  gtk4
                  libusb1
                  pkg-config
                ]
              )
            }:$LD_LIBRARY_PATH

            # Add pkg-config path for GTK4
            export PKG_CONFIG_PATH="${pkgs.gtk4}/lib/pkgconfig:$PKG_CONFIG_PATH"
          '';
        };
      }
    );
}
