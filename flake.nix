{
  inputs.configuration.url = "github:ivandimitrov8080/configuration.nix";
  outputs =
    { configuration
    , ...
    }:
    let
      system = "x86_64-linux";
      pkgs = import configuration.inputs.nixpkgs {
        inherit system; overlays = [
        configuration.overlays.default
        (final: prev: {
          font = prev.nerd-fonts.fira-code;
          nvim = prev.nvim.extend {
            plugins.lsp.servers = {
              clangd.enable = true;
            };
          };
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
        buildInputs = buildInputs ++ [ pkgs.nvim ];
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
