#include <iostream>
#include <limits>
#include <cstdio>
#include <cstring>
using namespace std;

struct est_livro {
    int data_emp;
    int data_dv;
    char usuario[40];
};

struct livro {
    int codigo_lv;
    int numero_p;
    char editora[40];
    char autores[40];
    char titulo_lv[40];
    char area[40];
    struct est_livro est_lv;
};

int main() {
    FILE *arquivos_livro;
    FILE *arquivos_livroAux;

    struct livro lv;
    int opcao = 0;

    while (opcao != 9) {
        cout << "=====================\n";
        cout << "   Menu da Biblioteca   \n";
        cout << "=====================\n";
        cout << "1 - Cadastro\n";
        cout << "2 - Alteracao\n";
        cout << "3 - Exclusao\n";
        cout << "4 - Emprestimo\n";
        cout << "5 - Devolucao\n";
        cout << "6 - Consulta de livro\n";
        cout << "7 - Livros disponiveis\n";
        cout << "8 - Listagem dos livros\n";
        cout << "9 - Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        system("cls");

        if (opcao == 1) {
            char opc_cd;
            cout << "Deseja cadastrar algum livro? (S/N): ";
            cin >> opc_cd;

            while (opc_cd == 'S' || opc_cd == 's') {
                arquivos_livro = fopen("livros.dates", "ab+");
                if (arquivos_livro == NULL) {
                    cout << "Erro ao abrir o arquivo.\n";
                    break;
                }

                cout << "Preencha os campos abaixo:\n";
                cout << "Area do livro: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get(lv.area, 40);
                cout << "Titulo do livro: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get(lv.titulo_lv, 40);
                cout << "Autores do livro: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get(lv.autores, 40);
                cout << "Editora do livro: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get(lv.editora, 40);
                cout << "Numero de paginas do livro: ";
                cin >> lv.numero_p;
                cout << "Codigo do livro: ";
                cin >> lv.codigo_lv;

                if (fwrite(&lv, sizeof(struct livro), 1, arquivos_livro) == 1) {
                    cout << "Registro gravado com sucesso!\n";
                } else {
                    cout << "Ops, ocorreu um erro!\n";
                }
                fclose(arquivos_livro);

                cout << "Deseja cadastrar outro livro? (S/N): ";
                cin >> opc_cd;
            }

        } else if (opcao == 2) {
            int busca_alt;
            cout << "Digite o codigo do livro que deseja alterar: ";
            cin >> busca_alt;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            arquivos_livro = fopen("livros.dates", "r+b");
            if (arquivos_livro == NULL) {
                cout << "Ops, erro ao abrir o arquivo.\n";
                cin.get();
                break;
            } else {
                int pos = -1;
                while (!feof(arquivos_livro)) {
                    fread(&lv, sizeof(struct livro), 1, arquivos_livro);
                    pos++;

                    if (busca_alt == lv.codigo_lv) {
                        fseek(arquivos_livro, sizeof(struct livro) * pos, SEEK_SET);
                    
                        cout << "Digite sua alteracao de titulo: ";
                        cin.get(lv.titulo_lv, 40);
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                       

                        if (fwrite(&lv, sizeof(struct livro), 1, arquivos_livro) == 1) {
                            cout << "Registro gravado com sucesso!\n";
                            break;
                        } else {
                            cout << "Ops, ocorreu um erro!\n";
                            break;
                        }
                    }
                }
                fclose(arquivos_livro);
            }

        } else if (opcao == 3) {
            int busca_excl;
            cout << "Informe o codigo do livro que deseja excluir: ";
            cin >> busca_excl;

            arquivos_livro = fopen("livros.dates", "rb");
            arquivos_livroAux = fopen("livros.aux", "wb");
            fread(&lv, sizeof(struct livro), 1, arquivos_livro);

            while (!feof(arquivos_livro)) {
                if (busca_excl != lv.codigo_lv) {
                    fwrite(&lv, sizeof(struct livro), 1, arquivos_livroAux);
                }
                fread(&lv, sizeof(struct livro), 1, arquivos_livro);
            }
            fclose(arquivos_livro);
            fclose(arquivos_livroAux);
            remove("livros.dates");
            rename("livros.aux", "livros.dates");

        } else if (opcao == 4) {
            int busca_em;
            cout << "Digite o codigo do livro que deseja emprestar: ";
            cin >> busca_em;

            arquivos_livro = fopen("livros.dates", "r+b");
            if (arquivos_livro == NULL) {
                cout << "Erro ao abrir o arquivo.\n";
                cin.get();
                break;
            } else {
                int pos = -1;
                while (!feof(arquivos_livro)) {
                    fread(&lv, sizeof(struct livro), 1, arquivos_livro);
                    pos++;
                    if (busca_em == lv.codigo_lv) {
                        fseek(arquivos_livro, sizeof(struct livro) * pos, SEEK_SET);
                        cout << "Digite a data do emprestimo: ";
                        cin >> lv.est_lv.data_emp;
                        cout << "Digite a data de devolucao: ";
                        cin >> lv.est_lv.data_dv;
                        cout << "Digite o nome do usuario: ";
                        cin >> lv.est_lv.usuario;

                        if (fwrite(&lv, sizeof(struct livro), 1, arquivos_livro) == 1) {
                            cout << "Emprestimo feito com sucesso!\n";
                            break;
                        } else {
                            cout << "Ops, ocorreu um erro!\n";
                        }
                    }
                }
            }
            fclose(arquivos_livro);

        } else if (opcao == 5) {
            int busca_dv;
            cout << "Digite o codigo do livro que deseja devolver: ";
            cin >> busca_dv;

            arquivos_livro = fopen("livros.dates", "rb+");
            if (arquivos_livro == NULL) {
                cout << "Ops, ocorreu um erro!\n";
            } else {
                int pos = -1;
                while (!feof(arquivos_livro)) {
                    fread(&lv, sizeof(struct livro), 1, arquivos_livro);
                    pos++;
                    if (busca_dv == lv.codigo_lv) {
                        fseek(arquivos_livro, sizeof(struct livro) * pos, SEEK_SET);
                        lv.est_lv.data_emp = 0;
                        lv.est_lv.data_dv = 0;
                        strcpy(lv.est_lv.usuario, " ");
                        if (fwrite(&lv, sizeof(struct livro), 1, arquivos_livro) == 1) {
                            cout << "Livro devolvido com sucesso!\n";
                        }
                        break;
                    }
                }
                fclose(arquivos_livro);
            }

        } else if (opcao == 6) {
            int busca;
            cout << "Informe o codigo do livro que deseja consultar: ";
            cin >> busca;

            arquivos_livro = fopen("livros.dates", "rb");
            if (arquivos_livro == NULL) {
                cout << "Ops, ocorreu um erro!\n";
            } else {
                while (!feof(arquivos_livro)) {
                    fread(&lv, sizeof(struct livro), 1, arquivos_livro);
                    if (busca == lv.codigo_lv) {
                        cout << "Area do livro: " << lv.area << endl;
                        cout << "Titulo do livro: " << lv.titulo_lv << endl;
                        cout << "Editora do livro: " << lv.editora << endl;
                        cout << "Autores do livro: " << lv.autores << endl;
                        cout << "Numero de paginas do livro: " << lv.numero_p << endl;
                        cout << "Codigo do livro: " << lv.codigo_lv << endl;
                        cout << "--------------------------------------------------------------\n";
                    }
                }
                fclose(arquivos_livro);
            }

        } else if (opcao == 7) {
            cout << "Livros disponiveis:\n";
            arquivos_livro = fopen("livros.dates", "rb");

            fread(&lv, sizeof(struct livro), 1, arquivos_livro);
            while (!feof(arquivos_livro)) {
                if (lv.est_lv.data_emp == 0) {
                    cout << "Area do livro: " << lv.area << endl;
                    cout << "Titulo do livro: " << lv.titulo_lv << endl;
                    cout << "Editora do livro: " << lv.editora << endl;
                    cout << "Autores do livro: " << lv.autores << endl;
                    cout << "Numero de paginas do livro: " << lv.numero_p << endl;
                    cout << "Codigo do livro: " << lv.codigo_lv << endl;
                    cout << "-----------------------------------------------------------------\n";
                }
                fread(&lv, sizeof(struct livro), 1, arquivos_livro);
            }
            fclose(arquivos_livro);

        } else if (opcao == 8) {
            cout << "Livros cadastrados:\n";
            arquivos_livro = fopen("livros.dates", "rb");
            if (arquivos_livro == NULL) {
                cout << "Ops, ocorreu um erro!\n";
            } else {
                fread(&lv, sizeof(struct livro), 1, arquivos_livro);
                while (!feof(arquivos_livro)) {
                    cout << "Area do livro: " << lv.area << endl;
                    cout << "Titulo do livro: " << lv.titulo_lv << endl;
                    cout << "Editora do livro: " << lv.editora << endl;
                    cout << "Autores do livro: " << lv.autores << endl;
                    cout << "Numero de paginas do livro: " << lv.numero_p << endl;
                    cout << "Codigo do livro: " << lv.codigo_lv << endl;
                    cout << "-------------------------------------------------------\n";
                    fread(&lv, sizeof(struct livro), 1, arquivos_livro);
                }
                fclose(arquivos_livro);
            }

        } else if (opcao == 9) {
            cout << "Ate mais!\n";
            break;
        }
    }

    cin.get();
    cin.ignore();
    
}
