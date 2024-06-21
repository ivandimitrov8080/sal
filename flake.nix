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
      nv = ide.nvim.${system}.standalone;
      pkgs = import nixpkgs {
        inherit system; overlays = [
        (final: prev: {
          nvim = nv.c { };
          font = pkgs.nerdfonts.override { fonts = [ "FiraCode" ]; };
        })
      ];
      };
      pname = "sal";
      version = "0.1.0";
      src = ./.;
      buildInputs = with pkgs; [
        raylib
        wayland
      ];
      nativeBuildInputs = with pkgs; [
        pkg-config
        makeBinaryWrapper
      ];
      env = {
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath buildInputs;
        FONT_PATH = "${pkgs.font}/share/fonts/truetype/NerdFonts/FiraCodeNerdFontMono-Bold.ttf";
      };
    in
    rec {
      devShells.${system}.default = pkgs.mkShell {
        inherit nativeBuildInputs env;
        buildInputs = buildInputs ++ (with pkgs; [ nvim ]);
      };
      packages.${system} = {
        default = pkgs.stdenv.mkDerivation {
          inherit buildInputs nativeBuildInputs pname version src env;
          installPhase = ''
            runHook preInstall
            make install
            wrapperfile=$out/bin/${pname}
            makeBinaryWrapper $out/bin/${pname}-unwrapped $wrapperfile --set LD_LIBRARY_PATH ${env.LD_LIBRARY_PATH} --set FONT_PATH ${env.FONT_PATH}
            runHook postInstall
          '';
        };
        font = pkgs.font;
      };
      overlays.default = final: prev: {
        sal = packages.${system}.default;
      };
    };
}
