{
  description = "A simple vector graphics editor for Allegro/DOS";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = rec {
          default = vect;

          vect = pkgs.stdenv.mkDerivation {
            pname = "vect";
            version = "1.0.0";

            src = ./.;

            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
            ];

            buildInputs = with pkgs; [
              allegro4
            ];
           };
        };
      }
    );
}
