{
  description = "C Template";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    ide = { url = "github:ivandimitrov8080/flake-ide"; inputs.nixpkgs.follows = "nixpkgs"; };
  };

  outputs =
    { nixpkgs
    , ide
    , ...
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system; overlays = [
        (final: prev: {
          nvim = ide.nvim.${system}.standalone.c { };
        })
      ];
      };
      pname = "hello-world";
      version = "0.0.1";
      src = ./.;
      buildInputs = with pkgs; [
        raylib
        wayland
      ];
      nativeBuildInputs = with pkgs; [
        # add build dependencies here
        ## For mesonbuild:
        #meson ninja
        ## For cmake:
        #cmake
        ## For autotools:
        # autoconf-archive
        # autoreconfHook
        pkg-config
      ];
      env = {
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath buildInputs;
      };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        inherit nativeBuildInputs env;
        buildInputs = buildInputs ++ (with pkgs; [ nvim entr ]);
      };
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        inherit buildInputs nativeBuildInputs pname version src env;
      };
    };
}
