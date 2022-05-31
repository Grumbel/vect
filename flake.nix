{
  description = "A simple vector graphics editor for Allegro/DOS";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          vect = pkgs.stdenv.mkDerivation {
            pname = "vect";
            version = "1.0.0";
            src = nixpkgs.lib.cleanSource ./.;
            enableParallelBuilding = true;
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.pkg-config
            ];
            buildInputs = [
              pkgs.allegro4
            ];
           };
        };
        defaultPackage = packages.vect;
      });
}
