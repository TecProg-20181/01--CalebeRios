#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

Image escala_de_cinza(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];

            media /= 3;

            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

void blur(Image *img, int tamanho) {
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_height = (img->height - 1 > i + tamanho/2) ? i + tamanho/2 : img->height - 1;
            int min_width = (img->width - 1 > j + tamanho/2) ? j + tamanho/2 : img->width - 1;
            for(int x = (0 > i - tamanho/2 ? 0 : i - tamanho/2); x <= menor_height; ++x) {
                for(int y = (0 > j - tamanho/2 ? 0 : j - tamanho/2); y <= min_width; ++y) {
                    media.r += img->pixel[x][y][0];
                    media.g += img->pixel[x][y][1];
                    media.b += img->pixel[x][y][2];
                }
            }

            media.r /= tamanho * tamanho;
            media.g /= tamanho * tamanho;
            media.b /= tamanho * tamanho;

            img->pixel[i][j][0] = media.r;
            img->pixel[i][j][1] = media.g;
            img->pixel[i][j][2] = media.b;
        }
    }
}

Image rotacionar_90_direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverter_cores(Image *img) {
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            img->pixel[i][j][0] = 255 - img->pixel[i][j][0];
            img->pixel[i][j][1] = 255 - img->pixel[i][j][1];
            img->pixel[i][j][2] = 255 - img->pixel[i][j][2];
        }
    }
}

Image cortar_imagem(Image img, int x, int y, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cortada;
}

void sepia(Image *img) {
  for (unsigned int x = 0; x < img->height; ++x) {
      for (unsigned int j = 0; j < img->width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = img->pixel[x][j][0];
          pixel[1] = img->pixel[x][j][1];
          pixel[2] = img->pixel[x][j][2];

          int p =  (pixel[0] * .393) + (pixel[1] * .769) + (pixel[2] * .189);
          int menor_r = (255 >  p) ? p : 255;
          img->pixel[x][j][0] = menor_r;

          p =  (pixel[0] * .349) + (pixel[1] * .686) + (pixel[2] * .168);
          menor_r = (255 >  p) ? p : 255;
          img->pixel[x][j][1] = menor_r;

          p =  (pixel[0] * .272) + (pixel[1] * .534) + (pixel[2] * .131);
          menor_r = (255 >  p) ? p : 255;
          img->pixel[x][j][2] = menor_r;
      }
  }
}

void espelhar(Image *img) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img->width, height = img->height;

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (int i2 = 0; i2 < height; ++i2) {
        for (int j = 0; j < height; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img->width - 1 - j;
            else x = img->height - 1 - i2;

            Pixel aux1;
            aux1.r = img->pixel[i2][j][0];
            aux1.g = img->pixel[i2][j][1];
            aux1.b = img->pixel[i2][j][2];

            img->pixel[i2][j][0] = img->pixel[x][y][0];
            img->pixel[i2][j][1] = img->pixel[x][y][1];
            img->pixel[i2][j][2] = img->pixel[x][y][2];

            img->pixel[x][y][0] = aux1.r;
            img->pixel[x][y][1] = aux1.g;
            img->pixel[x][y][2] = aux1.b;
        }
    }
}

void imprimir_imagem(Image *img) {
    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img->width, img->height);

    // print pixels of image
    for (unsigned int i = 0; i < img->height; ++i) {
        for (unsigned int j = 0; j < img->width; ++j) {
            printf("%hu %hu %hu ", img->pixel[i][j][0],
                                   img->pixel[i][j][1],
                                   img->pixel[i][j][2]);

        }
        printf("\n");
    }
}

Image definir_imagem() {
  Image img;

  // read type of image
  char p3[4];
  scanf("%s", p3);

  // read width height and color of image
  int max_color;
  scanf("%u %u %d", &img.width, &img.height, &max_color);

  // read all pixels of image
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j][0],
                               &img.pixel[i][j][1],
                               &img.pixel[i][j][2]);

      }
  }

  return img;
}

int main() {
    Image img;

    img = definir_imagem();

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                sepia(&img);
                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(&img, tamanho);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar_90_direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                espelhar(&img);
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(&img);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cortar_imagem(img, x, y, width, height);
                break;
            }
        }

    }

    imprimir_imagem(&img);

    return 0;
}
