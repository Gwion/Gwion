{ pkgs ? import <nixpkgs> {} }: pkgs.stdenv.mkDerivation rec {
  pname = "gwion";
  version = "0.1";

  src = pkgs.fetchFromGitHub {
    owner = "Gwion";
    repo = "Gwion";
    rev = "9dad0ff596eeafaf1a928deb8e0a6888e303b1cc";
    sha256 = "sha256-8qZzegpxMzGBuKwF0YyqOleATEmavf+ZzAV/GIx5FfE=";
    fetchSubmodules = true;
  };

  nativeBuildInputs = with pkgs; [
    git
  ];

  installPhase = ''
    mkdir -p $out/bin
    cp gwion $out/bin
  '';

  meta = with pkgs.lib; {
    description = "programming lang";
    homepage = "https://github.com/Gwion/Gwion";
    license = licenses.gpl3Plus;
    maintainers = ["pyrotek"];
    mainProgram = "gwion";
  };
}
