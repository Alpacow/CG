# Visualizador de imagens BMP

OBS: a imagem a ser manipulada deve estar na pasta "resources" do projeto, para alterar a imagem é necessário
alterar o nome da imagem em InitCanvas() na Controller.
"img = new Bmp(Utils::getImagePath("NOME_DA_IMAGEM.bmp"), &alerts);"

Funções obrigatórias
- girar 90 graus
- visualizar a imagem em tons de cinza
- visualizar componentes de cor separadamente
- reduzir pela metade (escala)

# funções extras
- espelhar a imagem vertical e horizontalmente
- imagem com efeito sépia
- imagem borrada
- salvar imagem com alterações no disco
- histograma de ocorrências dos canais RGB e da luminância
