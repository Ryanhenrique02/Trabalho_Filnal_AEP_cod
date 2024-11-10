#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "usuarios.txt"
#define KEY 5  

typedef struct {
    char username[50];
    char password[50];
} Usuario;

void crip(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] += KEY;
    }
}

// descriptografar uma string
void descrip(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] -= KEY;
    }
}

// adicionar um novo usuario
void ad() {
    Usuario usuario;
    FILE *file = fopen(FILENAME, "a");

    printf("Novo usuario:\n");
    printf("Nome de usuario: ");
    getchar();
    fgets(usuario.username, sizeof(usuario.username), stdin);
    usuario.username[strcspn(usuario.username, "\n")] = '\0';

    do {
        printf("Senha (deve ter mais de 8 caracteres): ");
        scanf("%s", usuario.password);
        if (strlen(usuario.password) <= 8) {
            printf("Senha fraca! Use mais de 8 caracteres.\n");
        }
    } while (strlen(usuario.password) <= 8);

    crip(usuario.password);

    fprintf(file, "%s,%s\n", usuario.username, usuario.password);
    fclose(file);

    printf("Usuario adicionado com sucesso.\n");
}

// listar todos os usuarios
void listarUsuarios() {
    Usuario usuario;
    FILE *file = fopen(FILENAME, "r");

    if (file == NULL) {
        printf("Nenhum usuario encontrado.\n");
        return;
    }

    printf("Lista de usuarios:\n");
    while (fscanf(file, "%[^,],%s\n", usuario.username, usuario.password) != EOF) {
        
        descrip(usuario.password);
        printf("Usuario: %s | Senha: %s\n", usuario.username, usuario.password);
    }

    fclose(file);
}

// alterar a senha de um usuario
void alterarUsuario() {
    Usuario usuario;
    char nome[50];
    char novaSenha[50];
    int encontrado = 0;

    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Nome de usuario para alterar: ");
    scanf("%s", nome);

    while (fscanf(file, "%[^,],%s\n", usuario.username, usuario.password) != EOF) {
        descrip(usuario.password);

        if (strcmp(usuario.username, nome) == 0) {
            do {
                printf("Nova senha (deve ter mais de 8 caracteres): ");
                scanf("%s", novaSenha);
                if (strlen(novaSenha) <= 8) {
                    printf("Senha fraca! Use mais de 8 caracteres.\n");
                }
            } while (strlen(novaSenha) <= 8);

            crip(novaSenha);
            fprintf(temp, "%s,%s\n", usuario.username, novaSenha);
            encontrado = 1;
        } else {
            crip(usuario.password);
            fprintf(temp, "%s,%s\n", usuario.username, usuario.password);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("Senha alterada com sucesso.\n");
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

// excluir um usuario
void excluir() {
    Usuario usuario;
    char nome[50];
    int encontrado = 0;

    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Nome de usuario para excluir: ");
    scanf("%s", nome);

    while (fscanf(file, "%[^,],%s\n", usuario.username, usuario.password) != EOF) {
        descrip(usuario.password);
        
        if (strcmp(usuario.username, nome) != 0) {
            crip(usuario.password);
            fprintf(temp, "%s,%s\n", usuario.username, usuario.password);
        } else {
            encontrado = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("Usuario excluido com sucesso.\n");
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

// Menu
int main() {
    int opcao;

    do {
        printf("\n=== Sistema de Gerenciamento de Usuarios ===\n");
        printf("1. Adicionar Usuario\n");
        printf("2. Listar Usuarios\n");
        printf("3. Alterar Senha do Usuario\n");
        printf("4. Excluir Usuario\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ad();
                break;
            case 2:
                listarUsuarios();
                break;
            case 3:
                alterarUsuario();
                break;
            case 4:
                excluir();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}