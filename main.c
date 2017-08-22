#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <locale.h>
#include <Windows.h>

#define _VALORDAMULTA 3

#define NAO 0
#define SIM 1

#define _STRNOME 100
#define _STRCPF 30
#define _STREMAIL 100
#define _STREND 200
#define _STRTEL 15

#define _STRTIT 100
#define _STRSUBTIT 100
#define _STRAUT 100
#define _STRISBN 50
#define _STREDITORA 50
#define _STREDICAO 50

#define _F_LETRAESPACO 0
#define _F_NUMERO 1
#define _F_EMAIL 2
#define _F_ENDERECO 3

#define _ASSOCIADO 0
#define _LIVRO 1
#define _EMPRESTIMO 2

struct s_associado
{
	short tipo;
	short deletado;

	int id_associado;
	char nome[_STRNOME];
	char cpf[_STRCPF];
	char email[_STREMAIL];
	char endereco[_STREND];
	char telefone[_STRTEL];
	int qnt_livro_emprestado;
	int multa;
	int dia_sem_livro;
	int qnt_de_multa;
	int bloqueado;
	int dia_do_bloqueio[3];
};

struct s_livro
{
	short tipo;
	short deletado;

	int id_livro;
	char titulo[_STRTIT];
	char subtitulo[_STRSUBTIT];
	char autor[_STRAUT];
	char isbn[_STRISBN];
	char editora[_STREDITORA];
	char edicao[_STREDICAO];
	int exemplar;
	int qnt_livro_emprestado;
	int reservadoParaIdAssociado;
};

struct s_emprestimo
{
	short tipo;
	short deletado;
	int id_emprestimo;
	int id_associado;
	int id_livro;
	int dataEmprestado[3];
	int dataDevolvido[3];
};

struct s_noh
{
	short tipo;
	short modificado;
	int offset;

	void *dado;

	struct s_noh *proximo;
	struct s_noh *anterior;
};

struct s_noh_ord
{
	char *nome;
	struct s_noh *endereco;

	struct s_noh_ord *proximo;
	struct s_noh_ord *anterior;
};

// SISTEMA
void splashScreen();
void inicializar();
void carregarMemoria();
void adicionarNoh(struct s_associado*, struct s_livro*, struct s_emprestimo*);
void gravarDisco();
int getTamanhoArquivo();
void limparTela();
void congelarTela();
int isLetra(char);
int isNumero(char);
char tirarAcento(unsigned char);
int getDia();
int getMes();
int getAno();
int diasEntreDuasDatas(int, int, int, int, int, int);

// MENU
int getOpcao(char*, char*);
int getSimNao();
char * getString(int, int);
int getNumero();
void mostrarTituloPequeno(char*);
void mostrarTituloGrande(char*);

// ASSOCIADO
void janelaCadastrarAssociado();
void janelaEditarAssociado();
void janelaRemoverAssociado();
void janelaBuscarAssociado();
void janelaMulta();
void janelaZerarMulta();
void janelaListarAssociado();
void janelaListarAssociadoOrdenado();
void QuickSortAssociado();
int getNovaIdAssociado();
struct s_noh * getNohAssociado();
int cmpStructNohOrd(const void*, const void*);
int tamString(char*);
void janelaInfoAssociado();
struct s_noh* getNohAssociadoByIdAssociado(int);

// LIVRO
void janelaCadastrarLivro();
void janelaEditarLivro();
void janelaRemoverLivro();
void janelaListarLivro();
int getNovaIdLivro();
struct s_noh * getNohLivroByIdLivro(int);
void janelaListarLivroOrdenado();
void QuickSortLivro();
struct s_noh* getNohLivroByIdLivro(int);

void reservarLivro();
void tirarReserva();

// EMPRESTIMO
void janelaEmprestarLivro();
void janelaDevolverLivro();
void janelaListarEmprestimo();
int getNovaIdEmprestimo();
struct s_noh * getNohEmprestimoByIdAssociado(int);
struct s_noh * getNohEmprestimoByIdLivro(int);

// VARIÁVEIS GLOBAIS
char *nomeDoArquivo = "booklending.db";
int offset;
struct s_noh *listaA;
struct s_noh *listaL;
struct s_noh *listaE;
struct s_noh_ord *listaAOrd;

int main()
{
	int opcao = 0;

	setlocale(LC_ALL, "");
	system("TITLE BookLending BETA");

	splashScreen();
	inicializar();
	carregarMemoria();

	do
	{
		opcao = getOpcao("Janela Principal", "0. Sair#1. Janela de controle de associados#2. Janela de controle de livros#3. Emprestar livro#4. Devolver livro#5. Listar empréstimos#$");
		switch (opcao)
		{
		case 0:
			break;
		case 1:
		MENUASSOCIADO:
			opcao = getOpcao("Controle de associados", "0. Voltar#1. Cadastrar associado#2. Editar associado#3. Remover associado#4. Buscar associado por nome#5. Ver multa de associado#6. Zerar multa de associado#7. Listar associados#8. Listar associados por ordem de nome#9. Informações de associado#$");
			switch (opcao)
			{
			case 0:
				opcao = (-1);
				break;
			case 1:
				janelaCadastrarAssociado();
				break;
			case 2:
				janelaEditarAssociado();
				break;
			case 3:
				janelaRemoverAssociado();
				break;
			case 4:
				janelaBuscarAssociado();
				break;
			case 5:
				janelaMulta();
				break;
			case 6:
				janelaZerarMulta();
				break;
			case 7:
				janelaListarAssociado();
				break;
			case 8:
				janelaListarAssociadoOrdenado();
				break;
			case 9:
				janelaInfoAssociado();
				break;
			default:
				goto MENUASSOCIADO;
				break;
			}
			break;
		case 2:
		MENULIVRO:
			opcao = getOpcao("Controle de livros", "0. Voltar#1. Cadastrar livro#2. Editar livro#3. Remover livro#4. Buscar livro#5. Listar livros#6. Listar livro ordenado#7. Reservar livro#8. Tirar reserva#$");

			switch (opcao)
			{
			case 0:
				opcao = (-1);
				break;
			case 1:
				janelaCadastrarLivro();
				break;
			case 2:
				janelaEditarLivro();
				break;
			case 3:
				janelaRemoverLivro();
				break;
			case 4:
				//janelaBuscarLivro();
				break;
			case 5:
				janelaListarLivro();
				break;
			case 6:
				//janelaListarLivroOrdenado();
				break;
			case 7:
				reservarLivro();
				break;
			case 8:
				tirarReserva();
				break;
			default:
				goto MENULIVRO;
				break;
			}
			break;
		case 3:
			janelaEmprestarLivro();
			break;
		case 4:
			janelaDevolverLivro();
			break;
		case 5:
			janelaListarEmprestimo();
			break;
		}
	} while (opcao != 0);
}
// *************************************************************************************************************************************
// *
// * SISTEMA
// *
// *************************************************************************************************************************************
void splashScreen()
{
	system("mode con lines=12");
	system("mode con cols=62");
	system("color 4F");

	limparTela();

	printf("\n");
	printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printf(" @                                                          @\n");
	printf(" @                                                          @\n");
	printf(" @                       BOOKLENDING 2012                   @\n");
	printf(" @                                                          @\n");
	printf(" @                                                          @\n");
	printf(" @  Mauricio Ize                                            @\n");
	printf(" @  Eduardo Farias                                          @\n");
	printf(" @                                                          @\n");
	printf(" @                                                          @\n");
	printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	Sleep(1000);

	limparTela();

	system("mode con lines=60");
	system("mode con cols=62");
	system("color 4F");
}
void inicializar()
{
	FILE *f;

	f = fopen(nomeDoArquivo, "rb");
	if (f == 0)
		f = fopen(nomeDoArquivo, "wb");

	if (f != 0)
		fclose(f);
}
void carregarMemoria()
{
	short tipo;
	int fr = 0;
	struct s_noh *tempA, *tempL, *tempE, *temp;
	FILE *f;

	listaA = (struct s_noh *) calloc(sizeof(struct s_noh), 1);
	listaL = (struct s_noh *) calloc(sizeof(struct s_noh), 1);
	listaE = (struct s_noh *) calloc(sizeof(struct s_noh), 1);
	listaAOrd = (struct s_noh_ord *) calloc(sizeof(struct s_noh_ord), 1);

	listaA->tipo = 0;
	listaA->modificado = 0;
	listaA->proximo = 0;
	listaA->anterior = 0;
	listaA->dado = 0;

	listaL->tipo = 0;
	listaL->modificado = 0;
	listaL->proximo = 0;
	listaL->anterior = 0;
	listaL->dado = 0;

	listaE->tipo = 0;
	listaE->modificado = 0;
	listaE->proximo = 0;
	listaE->anterior = 0;
	listaE->dado = 0;

	listaAOrd->proximo = 0;
	listaAOrd->endereco = 0;
	listaAOrd->nome = 0;

	tempA = listaA;
	tempL = listaL;
	tempE = listaE;

	f = fopen(nomeDoArquivo, "rb");

	if (f != 0)
	{
		while (!feof(f))
		{
			if ((fr = fread(&tipo, sizeof(short), 1, f)) != 0)
			{
				if (tipo == _ASSOCIADO)
				{
					fseek(f, (-sizeof(short)), SEEK_CUR);

					tempA->tipo = _ASSOCIADO;
					tempA->modificado = 0;
					tempA->offset = ftell(f);

					struct s_associado *associado = (struct s_associado*) calloc(sizeof(struct s_associado), 1);
					fread(associado, sizeof(struct s_associado), 1, f);
					offset = ftell(f);
					tempA->dado = (struct s_associado*) associado;

					temp = tempA;
					tempA->proximo = (struct s_noh*) calloc(sizeof(struct s_noh), 1);
					tempA = tempA->proximo;
					tempA->anterior = temp;
					tempA->tipo = 0;
					tempA->modificado = 0;
					tempA->proximo = 0;
					tempA->dado = 0;
					//tempA->anterior=0;
				}
				if (tipo == _LIVRO)
				{
					fseek(f, (-sizeof(short)), SEEK_CUR);
					tempL->tipo = _LIVRO;
					tempL->modificado = 0;
					tempL->offset = ftell(f);
					struct s_livro *livro = (struct s_livro*) calloc(sizeof(struct s_livro), 1);
					fread(livro, sizeof(struct s_livro), 1, f);
					offset = ftell(f);
					tempL->dado = (struct s_livro*) livro;
					temp = tempL;
					tempL->proximo = (struct s_noh*) calloc(sizeof(struct s_noh), 1);
					tempL = tempL->proximo;
					tempL->anterior = temp;
					tempL->tipo = 0;
					tempL->modificado = 0;
					tempL->proximo = 0;
					tempL->dado = 0;
					//tempL->anterior=0;
				}
				if (tipo == _EMPRESTIMO)
				{
					fseek(f, (-sizeof(short)), SEEK_CUR);
					tempE->tipo = _EMPRESTIMO;
					tempE->modificado = 0;
					tempE->offset = ftell(f);
					struct s_emprestimo *emprestimo = (struct s_emprestimo*) calloc(sizeof(struct s_emprestimo), 1);
					fread(emprestimo, sizeof(struct s_emprestimo), 1, f);
					offset = ftell(f);
					tempE->dado = (struct s_emprestimo*) emprestimo;
					temp = tempE;
					tempE->proximo = (struct s_noh*) calloc(sizeof(struct s_noh), 1);
					tempE = tempE->proximo;
					tempE->anterior = temp;
					tempE->tipo = 0;
					tempE->modificado = 0;
					tempE->proximo = 0;
					tempE->dado = 0;
					//tempE->anterior=0;
				}
			}
		}
	}
	if (f != 0)
		fclose(f);
}
void adicionarNoh(struct s_associado *a, struct s_livro *l, struct s_emprestimo *e)
{
	struct s_noh *temp, *temp2;

	if (a != 0)
	{
		for (temp = listaA; temp->proximo != 0; temp = temp->proximo);
		temp->dado = a;
		temp->tipo = _ASSOCIADO;
		temp->modificado = 1;
		temp->offset = offset;
		offset += sizeof(struct s_associado);
		temp->proximo = (struct s_noh*) calloc(sizeof(struct s_noh), 1);
		temp2 = temp;
		temp = temp->proximo;
		temp->anterior = temp2;
		temp->tipo = 0;
		temp->modificado = 0;
		temp->proximo = 0;
		temp->dado = 0;
		temp->offset = 0;
		//temp->anterior=0;
	}
	if (l != 0)
	{
		for (temp = listaL; temp->proximo != 0; temp = temp->proximo);
		temp->dado = l;
		temp->tipo = _LIVRO;
		temp->modificado = 1;
		temp->offset = offset;
		offset += sizeof(struct s_livro);
		temp->proximo = (struct s_noh*) calloc(sizeof(struct s_noh), 1);
		temp2 = temp;
		temp = temp->proximo;
		temp->anterior = temp2;
		temp->tipo = 0;
		temp->modificado = 0;
		temp->proximo = 0;
		temp->dado = 0;
		temp->offset = 0;
		//temp->anterior=0;
	}
	if (e != 0)
	{
		for (temp = listaE; temp->proximo != 0; temp = temp->proximo);
		temp->dado = e;
		temp->tipo = _EMPRESTIMO;
		temp->modificado = 1;
		temp->offset = offset;
		offset += sizeof(struct s_emprestimo);
		temp->proximo = (struct s_noh*) calloc(sizeof(struct s_noh), 1);
		temp2 = temp;
		temp = temp->proximo;
		temp->anterior = temp2;
		temp->tipo = 0;
		temp->modificado = 0;
		temp->proximo = 0;
		temp->dado = 0;
		temp->offset = 0;
		//temp->anterior=0;
	}
	gravarDisco();
}
void gravarDisco()
{
	struct s_noh *temp;
	FILE *f;

	for (temp = listaA; temp->proximo != 0; temp = temp->proximo)
	{
		if (temp->modificado == 1)
		{
			f = fopen(nomeDoArquivo, "r+b");
			if (f != 0)
			{
				temp->modificado = 0;
				fseek(f, temp->offset, SEEK_SET);
				fwrite(temp->dado, sizeof(struct s_associado), 1, f);
				fclose(f);
			}
		}
	}
	for (temp = listaL; temp->proximo != 0; temp = temp->proximo)
	{
		if (temp->modificado == 1)
		{
			f = fopen(nomeDoArquivo, "r+b");
			if (f != 0)
			{
				temp->modificado = 0;
				fseek(f, temp->offset, SEEK_SET);
				fwrite(temp->dado, sizeof(struct s_livro), 1, f);
				fclose(f);
			}
		}
	}
	for (temp = listaE; temp->proximo != 0; temp = temp->proximo)
	{
		if (temp->modificado == 1)
		{
			f = fopen(nomeDoArquivo, "r+b");
			if (f != 0)
			{
				temp->modificado = 0;
				fseek(f, temp->offset, SEEK_SET);
				fwrite(temp->dado, sizeof(struct s_emprestimo), 1, f);
				fclose(f);
			}
		}
	}
}
int getTamanhoArquivo()
{
	int tam = 0;
	FILE *f;

	f = fopen(nomeDoArquivo, "rb");
	if (f != 0)
	{
		fseek(f, 0, SEEK_END);
		tam = ftell(f);
		fclose(f);
	}

	return tam;
}
void limparTela()
{
	system("cls");
}
void congelarTela()
{
	char ch;

	printf("Aperte espaço para continuar...\n");
	do
	{
		fflush(stdin);
		ch = getch();
	} while (ch != ' ');
}
int isLetra(char ch)
{
	if ((ch >= 97) && (ch <= 122))
		return 1;
	if ((ch >= 65) && (ch <= 90))
		return 1;

	return 0;
}
int isNumero(char ch)
{
	if ((ch >= 48) && (ch <= 57))
		return 1;

	return 0;
}
char tirarAcento(unsigned char ch)
{
	// 198 se refere ao ã no ascii extended, que é o que getch e scanf pegam

	if (ch == 135) // ç
		return('C');

	if (ch == 198) // ã
		return('A');
	if (ch == 199) // Ã
		return('A');
	if (ch == 131) // â
		return('A');
	if (ch == 182) // Â
		return('A');
	if (ch == 160) // á
		return('A');
	if (ch == 181) // Á
		return('A');
	if (ch == 133) // à
		return('A');
	if (ch == 183) // À
		return('A');
	if (ch == 132) // ä
		return('A');
	if (ch == 142) // Ä
		return('A');

	if (ch == 130) // é
		return('E');
	if (ch == 144) // É
		return('E');
	if (ch == 138) // è
		return('E');
	if (ch == 212) // È
		return('E');
	if (ch == 136) // ê
		return('E');
	if (ch == 210) // Ê
		return('E');
	if (ch == 137) // ë
		return('E');
	if (ch == 211) // Ë
		return('E');

	if (ch == 161) // í
		return('I');
	if (ch == 214) // Í
		return('I');
	if (ch == 141) // ì
		return('I');
	if (ch == 222) // Ì
		return('I');

	if (ch == 162) // ó
		return('O');
	if (ch == 224) // Ó
		return('O');
	if (ch == 149) // ò
		return('O');
	if (ch == 227) // Ò
		return('O');
	if (ch == 147) // ô
		return('O');
	if (ch == 226) // Ô
		return('O');
	if (ch == 228) // õ
		return('O');
	if (ch == 229) // Õ
		return('O');
	if (ch == 148) // ö
		return('O');
	if (ch == 153) // Ö
		return('O');

	if (ch == 163) // ú
		return('U');
	if (ch == 233) // Ú
		return('U');
	if (ch == 151) // ù
		return('U');
	if (ch == 235) // Ù
		return('U');
	if (ch == 129) // ü
		return('U');
	if (ch == 154) // Ü
		return('U');

	return ch;
}
int getDia()
{
	time_t timer = time(NULL);
	struct tm *now = localtime(&timer);

	return now->tm_mday;
}
int getMes()
{
	time_t timer = time(NULL);
	struct tm *now = localtime(&timer);

	return now->tm_mon + 1;
}
int getAno()
{
	time_t timer = time(NULL);
	struct tm *now = localtime(&timer);

	return now->tm_year + 1900;
}
int totalDiasMes(int mes, int ano)
{
	switch (mes)
	{
	case 0: case 2: case 4: case 6: case 7: case 9: case 11:
		return 31;
		break;

	case 3: case 5: case 8: case 10:
		return 30;
		break;

	case 1:
		if (!(ano % 4))
			return 29;
		else
			return 28;
		break;

	default:
		return 0;
		break;
	}
}
int diasEntreDuasDatas(int diaIni, int mesIni, int anoIni, int diaFim, int mesFim, int anoFim)
{
	int dI = diaIni, mI = mesIni;
	int aI = anoIni;
	int cDias = 0;
	int qtdeDiasMes;

	if (!((diaIni == diaFim) && (mesIni == mesFim) && (anoIni == anoFim)))
		do {
			qtdeDiasMes = totalDiasMes(mI, aI);
			if (dI == qtdeDiasMes)
			{
				dI = 1; ++mI;
				++cDias;
				if (mI > 11)
				{
					mI = 0; ++aI;
					qtdeDiasMes = totalDiasMes(mI, aI);
				}
				else
					qtdeDiasMes = totalDiasMes(mI, aI);
			}

			if (!((dI == diaFim) && (mI == mesFim) && (aI == anoFim)))
				for (; dI < qtdeDiasMes; dI++)
				{
					++cDias;
					if ((dI == diaFim) && (mI == mesFim) && (aI == anoFim))
					{
						--cDias;
						break;
					}
				}
		} while (!((dI == diaFim) && (mI == mesFim) && (aI == anoFim)));

		return cDias;
}
// *************************************************************************************************************************************
// *
// * MENU
// *
// *************************************************************************************************************************************
int getOpcao(char *titulo, char *texto)
{
	char *pergunta;	int pos, pos2, tam = 0, ini = 0;

	limparTela();

	printf("\n");
	mostrarTituloPequeno("Sistema de controle de empréstimo de livros");
	printf("\n");
	printf("    %d/%d/%d\n", getDia(), getMes(), getAno());
	printf("\n");
	printf(" +==========================================================+\n");
	printf(" |                                                          |\n");
	printf(" |   %-54s |\n", titulo);
	printf(" |                                                          |\n");

	for (pos = 0; texto[pos] != '$'; pos++)
	{
		if (texto[pos] == '#')
		{
			pergunta = (char*)malloc(tam + 1);
			for (pos2 = 0; pos2<tam; pos2++)
				pergunta[pos2] = texto[ini + pos2];
			pergunta[pos2] = '\0';
			printf(" |      %-51s |\n", pergunta);
			tam = 0;
			ini = pos + 1;
			free(pergunta);
		}
		else
			tam++;
	}
	printf(" |                                                          |\n");
	printf(" +==========================================================+\n");
	printf("\n");

	return(getNumero());
}
int getSimNao()
{
	return 0;
}
char * getString(int tamanho, int filtro)
{
	char *bkp, *str; int i, j;

	bkp = (char*)calloc(sizeof(char), tamanho + 1);
	str = (char*)calloc(sizeof(char), tamanho + 1);

	fflush(stdin);
	fgets(bkp, tamanho, stdin);

	if (filtro == _F_LETRAESPACO)
	{
		for (i = 0, j = 0; i <= tamanho; i++, j++)
		{
			bkp[i] = tirarAcento(bkp[i]);
			if (isLetra(bkp[i]))
			{
				str[j] = toupper(bkp[i]);
			}
			else if (bkp[i] == ' ')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '\n' || bkp[i] == '\0' || bkp[i] == '\r')
			{
				str[j] = '\0';
				break;
			}
			else
				j--;
		}
	}
	if (filtro == _F_NUMERO)
	{
		for (i = 0, j = 0; i <= tamanho; i++, j++)
		{
			if (isNumero(bkp[i]))
				str[j] = bkp[i];
			else if (bkp[i] == '\n' || bkp[i] == '\0' || bkp[i] == '\r')
			{
				str[j] = '\0';
				break;
			}
			else
				j--;
		}
	}
	if (filtro == _F_EMAIL)
	{
		for (i = 0, j = 0; i <= tamanho; i++, j++)
		{
			bkp[i] = tirarAcento(bkp[i]);
			if (isLetra(bkp[i]))
			{
				str[j] = toupper(bkp[i]);
			}
			else if (isNumero(bkp[i]))
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == ' ')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '@')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '.')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '_')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '-')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '\n' || bkp[i] == '\0' || bkp[i] == '\r')
			{
				str[j] = '\0';
				break;
			}
			else
				j--;
		}
	}
	if (filtro == _F_ENDERECO)
	{
		for (i = 0, j = 0; i <= tamanho; i++, j++)
		{
			bkp[i] = tirarAcento(bkp[i]);
			if (isLetra(bkp[i]))
			{
				str[j] = toupper(bkp[i]);
			}
			else if (isNumero(bkp[i]))
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == ' ')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == ',')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '.')
			{
				str[j] = bkp[i];
			}
			else if (bkp[i] == '\n' || bkp[i] == '\0' || bkp[i] == '\r')
			{
				str[j] = '\0';
				break;
			}
			else
				j--;
		}
	}

	return str;
}
int getNumero()
{
	char ch[2]; int numero;

	do
	{
		fflush(stdin);
		ch[0] = getch();
		ch[1] = 0;
	} while (!isNumero(ch[0]));

	numero = atoi(ch);

	return numero;
}
void mostrarTituloPequeno(char *titulo)
{
	limparTela();
	printf("\n");
	printf(" +==========================================================+\n");
	printf(" |                                                          |\n");
	printf(" |   %-54s |\n", titulo);
	printf(" |                                                          |\n");
	printf(" +==========================================================+\n");
	printf("\n");
}
void mostrarTituloGrande(char *titulo)
{
	printf(" +------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
	printf(" |                                                                                                                                                            |\n");
	printf(" |   %-152s |\n", titulo);
	printf(" |                                                                                                                                                            |\n");
	printf(" +------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
}
// *************************************************************************************************************************************
// *
// * ASSOCIADO
// *
// *************************************************************************************************************************************
void janelaCadastrarAssociado()
{
	/*
	short tipo;
	short deletado;
	int id_associado;
	char nome[_STRNOME];
	char cpf[_STRCPF];
	char email[_STREMAIL];
	char endereco[_STREND];
	char telefone[_STRTEL];
	int qnt_livro_emprestado;
	int multa;
	int dia_sem_livro;
	int qnt_de_multa;
	int bloqueado;
	int dia_do_bloqueio[3];
	*/

	struct s_noh *temp;
	struct s_associado *associado;
	associado = (struct s_associado*) calloc(sizeof(struct s_associado), 1);

	associado->tipo = _ASSOCIADO;
	associado->deletado = 0;
	associado->id_associado = getNovaIdAssociado();

	associado->qnt_livro_emprestado = 0;
	associado->multa = 0;
	associado->dia_sem_livro = 0;
	associado->qnt_de_multa = 0;
	associado->bloqueado = 0;
	associado->dia_do_bloqueio[0] = 0;
	associado->dia_do_bloqueio[1] = 1;
	associado->dia_do_bloqueio[2] = 2;

	// TOPO
	limparTela();
	printf("\n");
	mostrarTituloPequeno("Cadastro de associado");
	printf("\n");

	// PERGUNTAS
	printf("  ID: %d", associado->id_associado);
	printf("\n");

	printf("  Digite o CPF do associado:\n  > ");
	strcpy(associado->cpf, getString(_STRCPF, _F_NUMERO));
	printf("\n");

	for (temp = listaA; temp->proximo != 0; temp = temp->proximo) {
		if ((strcmp(associado->cpf, ((struct s_associado*)temp->dado)->cpf)) == 0)
		{
			printf("ASSOCIADO JA CADASTRADO\n\n"); congelarTela(); return;
		}
	}

	printf("  Digite o nome do associado:\n  > ");
	strcpy(associado->nome, getString(_STRNOME, _F_LETRAESPACO));
	printf("\n");

	printf("  Digite o e-mail do associado:\n  > ");
	strcpy(associado->email, getString(_STREMAIL, _F_EMAIL));
	printf("\n");

	printf("  Digite o endereço do associado:\n  > ");
	strcpy(associado->endereco, getString(_STREND, _F_ENDERECO));
	printf("\n");

	printf("  Digite o telefone do associado:\n  > ");
	strcpy(associado->telefone, getString(_STRTEL, _F_NUMERO));
	printf("\n");

	// CONFIRMAÇÃO
	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Cadastro de associado");
		printf("\n");

		printf("   Nome......: %s\n", associado->nome);
		printf("   CPF.......: %s\n", associado->cpf);
		printf("   E-mail....: %s\n", associado->email);
		printf("   Endereço..: %s\n", associado->endereco);
		printf("   Telefone..: %s\n", associado->telefone);
		printf("\n");
		printf(" [0=Cancelar] [1=Cadastrar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			adicionarNoh(associado, 0, 0);
			return;
		}
	}
}
void janelaEditarAssociado()
{
	struct s_noh *temp;

	int id_associado;

	struct s_associado *associado;
	associado = (struct s_associado*) calloc(sizeof(struct s_associado), 1);

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Editar associado");
	printf("\n");

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");

	temp = getNohAssociadoByIdAssociado(id_associado);

	if (temp == 0 || (((struct s_associado*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return;
	}

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Editar associado");
	printf("\n");

	printf("   Nome......: %s\n", ((struct s_associado*)temp->dado)->nome);
	printf("   CPF.......: %s\n", ((struct s_associado*)temp->dado)->cpf);
	printf("   E-mail....: %s\n", ((struct s_associado*)temp->dado)->email);
	printf("   Endereço..: %s\n", ((struct s_associado*)temp->dado)->endereco);
	printf("   Telefone..: %s\n", ((struct s_associado*)temp->dado)->telefone);
	printf("\n");
	printf(" [0=Cancelar] [1=Continuar]\n");

	if (getNumero() == 0)
	{
		limparTela();
		printf("\n  Cancelado\n");
		congelarTela();
		return;
	}

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Editar associado");
	printf("\n");

	printf("   Nome......: %s\n", ((struct s_associado*)temp->dado)->nome);
	printf("   CPF.......: %s\n", ((struct s_associado*)temp->dado)->cpf);
	printf("   E-mail....: %s\n", ((struct s_associado*)temp->dado)->email);
	printf("   Endereço..: %s\n", ((struct s_associado*)temp->dado)->endereco);
	printf("   Telefone..: %s\n", ((struct s_associado*)temp->dado)->telefone);
	printf("\n");

	printf("  Digite o nome do associado:\n  > ");
	strcpy(associado->nome, getString(_STRNOME, _F_LETRAESPACO));
	if (strcmp(associado->nome, "") == 0)
	{
		strcpy(associado->nome, ((struct s_associado*)temp->dado)->nome);
		printf("  > %s\n", associado->nome);
	}
	printf("\n");

	printf("  Digite o CPF do associado:\n  > ");
	strcpy(associado->cpf, getString(_STRCPF, _F_NUMERO));
	if (strcmp(associado->cpf, "") == 0)
	{
		strcpy(associado->cpf, ((struct s_associado*)temp->dado)->cpf);
		printf("  > %s\n", associado->cpf);
	}
	printf("\n");

	printf("  Digite o e-mail do associado:\n  > ");
	strcpy(associado->email, getString(_STREMAIL, _F_EMAIL));
	if (strcmp(associado->email, "") == 0)
	{
		strcpy(associado->email, ((struct s_associado*)temp->dado)->email);
		printf("  > %s\n", associado->email);
	}
	printf("\n");

	printf("  Digite o endereço do associado:\n  > ");
	strcpy(associado->endereco, getString(_STREND, _F_ENDERECO));
	if (strcmp(associado->endereco, "") == 0)
	{
		strcpy(associado->endereco, ((struct s_associado*)temp->dado)->endereco);
		printf("  > %s\n", associado->endereco);
	}
	printf("\n");

	printf("  Digite o telefone do associado:\n  > ");
	strcpy(associado->telefone, getString(_STRTEL, _F_NUMERO));
	if (strcmp(associado->telefone, "") == 0)
	{
		strcpy(associado->telefone, ((struct s_associado*)temp->dado)->telefone);
		printf("  > %s\n", associado->telefone);
	}
	printf("\n");

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Editar associado");
		printf("\n");

		printf("   Dados antigos:\n");
		printf("   Nome......: %s\n", ((struct s_associado*)temp->dado)->nome);
		printf("   CPF.......: %s\n", ((struct s_associado*)temp->dado)->cpf);
		printf("   E-mail....: %s\n", ((struct s_associado*)temp->dado)->email);
		printf("   Endereço..: %s\n", ((struct s_associado*)temp->dado)->endereco);
		printf("   Telefone..: %s\n", ((struct s_associado*)temp->dado)->telefone);
		printf("\n");

		printf("   Dados modificados:\n");
		printf("   Nome......: %s\n", associado->nome);
		printf("   CPF.......: %s\n", associado->cpf);
		printf("   E-mail....: %s\n", associado->email);
		printf("   Endereço..: %s\n", associado->endereco);
		printf("   Telefone..: %s\n", associado->telefone);
		printf("\n");
		printf(" [0=Cancelar] [1=Editar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			strcpy(((struct s_associado*)temp->dado)->nome, associado->nome);
			strcpy(((struct s_associado*)temp->dado)->cpf, associado->cpf);
			strcpy(((struct s_associado*)temp->dado)->email, associado->email);
			strcpy(((struct s_associado*)temp->dado)->endereco, associado->endereco);
			strcpy(((struct s_associado*)temp->dado)->telefone, associado->telefone);
			temp->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void janelaRemoverAssociado()
{
	struct s_noh *temp;

	int id_associado;

	struct s_associado *associado;
	associado = (struct s_associado*) calloc(sizeof(struct s_associado), 1);

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Remover associado");
	printf("\n");

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");

	temp = getNohAssociadoByIdAssociado(id_associado);

	if (temp == 0 || (((struct s_associado*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return;
	}


	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Remover associado");
		printf("\n");

		printf("   Nome......: %s\n", ((struct s_associado*)temp->dado)->nome);
		printf("   CPF.......: %s\n", ((struct s_associado*)temp->dado)->cpf);
		printf("   E-mail....: %s\n", ((struct s_associado*)temp->dado)->email);
		printf("   Endereço..: %s\n", ((struct s_associado*)temp->dado)->endereco);
		printf("   Telefone..: %s\n", ((struct s_associado*)temp->dado)->telefone);
		printf("\n");
		printf(" [0=Cancelar] [1=Remover]\n");

		if (getNumero() == 0)
		{
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		}
		if (getNumero() == 1)
		{
			((struct s_associado*)temp->dado)->deletado = 1;
			temp->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void janelaListarAssociado()
{
	struct s_noh *temp;
	int itemTotal = 0, pagTotal = 0, pagAtual = 1, op = (-1), count, itemPorPagina = 40;

	temp = listaA;

	if (temp->proximo == 0)
	{
		limparTela();
		printf("Não foram encontrados registros\n");
		congelarTela();
		return;
	}

	// Descobre quantos itens existem na lista encadeada (1 item = 1)
	for (temp = listaA, itemTotal = 0; temp->proximo != 0; temp = temp->proximo, itemTotal++)
		if (((struct s_associado*)temp->dado)->deletado == 1)
			itemTotal--;

	// Calcula o número de páginas necessárias
	pagTotal = itemTotal / itemPorPagina;
	if ((itemTotal%itemPorPagina) != 0)
		pagTotal++;

	// Ajusta a tela
	system("mode con lines=54");
	system("mode con cols=160");

	// + ---------------------------------------------------------- +

	temp = listaA;
	while (op != 0)
	{
		printf("\n");
		mostrarTituloGrande("Lista de associados");
		printf("\n");

		printf("  +------+--------------------------------+-----------------+---------------------------------+-----------------------------------------------+--------------+\n");
		printf("  | ID   | NOME                           | CPF             | E-MAIL                          | ENDEREÇO                                      | TELEFONE     |\n");
		printf("  +------+--------------------------------+-----------------+---------------------------------+-----------------------------------------------+--------------+\n");
		for (count = 1; count <= itemPorPagina; count++)
		{
			if (temp->proximo != 0)
			{
				if (((struct s_associado*)temp->dado)->deletado != 1)
				{
					printf("  | %-04d |", ((struct s_associado*)temp->dado)->id_associado);
					printf(" %-30s |", ((struct s_associado*)temp->dado)->nome);
					printf(" %-15s |", ((struct s_associado*)temp->dado)->cpf);
					printf(" %-31s |", ((struct s_associado*)temp->dado)->email);
					printf(" %-45s |", ((struct s_associado*)temp->dado)->endereco);
					printf(" %-12s |", ((struct s_associado*)temp->dado)->telefone);
					printf(" \n");
				}
				else
					count--;

				temp = temp->proximo;
			}
			else
				printf("  |      |                                |                 |                                 |                                               |              |\n");
		}
		printf("  +------+--------------------------------+-----------------+---------------------------------+-----------------------------------------------+--------------+\n");
		printf("\n");
		printf(" [0=Sair][1=Página anterior][2=Próxima página]    [página %d de %d]    [%d associado(s)]\n", pagAtual, pagTotal, itemTotal);

		// + ---------------------------------------------------------- +
		while (op != 0)
		{
			op = getNumero();
			if (op == 0)
				break;

			if (op == 1)
			{
				if (pagAtual>1)
				{
					int aux, v;

					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) != 0))
						v = itemPorPagina + (itemTotal - ((pagAtual - 1)*itemPorPagina));

					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) == 0))
						v = (2 * itemPorPagina);

					if ((pagAtual<pagTotal))
						v = (2 * itemPorPagina);

					for (aux = 1; aux <= v; aux++)
						temp = temp->anterior;

					pagAtual--;
					limparTela();
					break;
				}
			}
			if (op == 2)
			{
				if (pagAtual<pagTotal)
				{
					pagAtual++;
					limparTela();
					break;
				}
			}
		}
		// + ---------------------------------------------------------- +
	}

	system("mode con lines=60");
	system("mode con cols=62");
}
void janelaMulta()
{
	struct s_noh *temp;

	int id_associado;

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Multa");
	printf("\n");

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");

	temp = getNohAssociadoByIdAssociado(id_associado);

	if (temp == 0)
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return;
	}

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Multa");
		printf("\n");

		printf("   O associado %s possui %f reais em multa.\n", ((struct s_associado*)temp->dado)->nome, ((struct s_associado*)temp->dado)->multa);
		printf("\n");
		printf(" [0=Sair]\n");
		if (getNumero() == 0)
			return;
	}
}
void janelaZerarMulta()
{
	struct s_noh *temp; int opcao;

	int id_associado;

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Multa");
	printf("\n");

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");

	temp = getNohAssociadoByIdAssociado(id_associado);

	if (temp == 0)
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return;
	}

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Multa");
		printf("\n");

		printf("   O associado %s possui %f reais em multa.\n", ((struct s_associado*)temp->dado)->nome, ((struct s_associado*)temp->dado)->multa);
		printf("\n");
		printf(" [0=Sair] [1=Zerar multa]\n");
		opcao = getNumero();
		if (opcao == 0)
			return;
		if (opcao == 1)
		{
			((struct s_associado*)temp->dado)->multa;
			temp->modificado = 1;
			gravarDisco();
		}
	}
}
int getNovaIdAssociado()
{
	int id = 1; struct s_noh *temp;

	for (temp = listaA; temp->proximo != 0; temp = temp->proximo, id++);

	for (temp = listaA; temp->proximo != 0; temp = temp->proximo)
		if (id == ((struct s_associado*)temp->dado)->id_associado)
		{
			id++;
			temp = listaA;
		}

	return id;
}
void janelaInfoAssociado()
{
	struct s_noh *temp;

	int id_associado;

	struct s_associado *associado;


	limparTela();
	printf("\n");
	mostrarTituloPequeno("Informações de associado");
	printf("\n");

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");

	temp = getNohAssociadoByIdAssociado(id_associado);

	if (temp == 0 || (((struct s_associado*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return;
	}


	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Informações de associado");
		printf("\n");

		printf("   Nome......: %s\n", ((struct s_associado*)temp->dado)->nome);
		printf("   CPF.......: %s\n", ((struct s_associado*)temp->dado)->cpf);
		printf("   E-mail....: %s\n", ((struct s_associado*)temp->dado)->email);
		printf("   Endereço..: %s\n", ((struct s_associado*)temp->dado)->endereco);
		printf("   Telefone..: %s\n", ((struct s_associado*)temp->dado)->telefone);
		printf("\n");
		if (((struct s_associado*)temp->dado)->bloqueado == 1)
		{
			int diasBloqueado = diasEntreDuasDatas(getDia(), getMes() - 1, getAno(), ((struct s_associado*)temp->dado)->dia_do_bloqueio[0], ((struct s_associado*)temp->dado)->dia_do_bloqueio[1] - 1, ((struct s_associado*)temp->dado)->dia_do_bloqueio[2]);
			printf("  Bloqueado: SIM\n");
			printf("  Dias bloqueado: %d\n", diasBloqueado);
			printf("  Dias restante: %d\n", 180 - diasBloqueado);
		}
		else
		{
			printf("  Bloqueado: NÃO\n");
		}
		printf("\n");
		printf("   Multa: %d\n", ((struct s_associado*)temp->dado)->multa);
		printf("\n");
		printf("   Quantidade de multa: %d\n", ((struct s_associado*)temp->dado)->qnt_de_multa);
		printf("\n");
		printf(" [0=Cancelar]\n");

		if (getNumero() == 0)
		{
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		}
	}
}
// ************************************************ QUICKSORT & BUSCA BINÁRIA
int busca(struct s_noh_ord *vetor, int tam, char *elemento) {
	int i;
	for (i = 0; i<tam; i++) {
		if (strcmp(vetor[i].nome, elemento) == 0)
			return i;
		else if (strcmp(vetor[i].nome, elemento) > 0)
			return -2;
	}
	return -1;
}
void janelaBuscarAssociado()
{
	int qnt, i;
	struct s_noh *temp;
	struct s_noh_ord *tempO, *tempO2;

	// vetor de ponteiros para noh
	struct s_noh_ord *vetor;

	// conta quantos nomes há
	for (qnt = 0, temp = listaA; temp->proximo != 0; temp = temp->proximo)
		if (((struct s_associado*)temp->dado)->deletado != 1)
			qnt++;

	// aloca memoria para QNT noh's
	vetor = (struct s_noh_ord *) calloc(sizeof(struct s_noh_ord), qnt);

	// copia os nomes para os noh's
	for (i = 0, temp = listaA; temp->proximo != 0; temp = temp->proximo, i++)
	{
		if (((struct s_associado*)temp->dado)->deletado != 1)
		{
			vetor[i].endereco = temp;
			vetor[i].nome = (char *)calloc(tamString((char*)&((struct s_associado*)temp->dado)->nome), sizeof(char));
			strcpy(vetor[i].nome, ((struct s_associado*)temp->dado)->nome);
			vetor[i].proximo = 0;
		}
		else
			i--;
	}

	// qsort neles
	qsort(vetor, qnt, sizeof(struct s_noh_ord), cmpStructNohOrd);

	char termo[_STRNOME]; char *result;
	printf("Digite o nome para procurar:\n");
	strcpy(termo, getString(_STRNOME, _F_LETRAESPACO));

	char *keey; char **key;
	keey = termo;
	key = &keey;
	/*result = (char*) bsearch(key,vetor,qnt,sizeof(struct s_noh_ord),cmpStructNohOrd);

	if(result>0)
	printf("Encontrado:  %s\n",vetor[result]);
	else
	printf("nao encontrado - %d\n",result);*/
	int ii = busca(vetor, qnt, termo);
	if (ii >= 0)
		printf("Encontrado:  %s \n", vetor[ii]);
	else
		printf("Não encontrado\n");
	congelarTela();



}
void janelaListarAssociadoOrdenado()
{
	QuickSortAssociado();
	//*************************************************************************************************
	struct s_noh_ord *temp;
	int itemTotal = 0, pagTotal = 0, pagAtual = 1, op = (-1), count, itemPorPagina = 40;

	// Verifica se existe registros para mostrar
	//*************************************************************************************************
	temp = listaAOrd;
	if (temp->proximo == 0)
	{
		limparTela();
		printf("Não foram encontrados registros\n");
		congelarTela();
		return;
	}

	// Descobre quantos itens existem na lista encadeada (1 item = 1)
	//*************************************************************************************************
	for (temp = listaAOrd, itemTotal = 0; temp->proximo != 0; temp = temp->proximo, itemTotal++)
		//if(((struct s_associado*)temp->dado)->deletado==1)
		//	itemTotal--;

		// Calcula o número de páginas necessárias
		pagTotal = itemTotal / itemPorPagina;
	if ((itemTotal%itemPorPagina) != 0)
		pagTotal++;

	// Ajusta a tela
	system("mode con lines=54");
	system("mode con cols=160");

	//*************************************************************************************************
	temp = listaAOrd;

	while (op != 0)
	{

		printf("\n");
		mostrarTituloGrande("Lista de associados");
		printf("\n");
		printf("  +------+---------------------------------------+------------------+----------------------+-------------------------------------------------+---------------+\n");
		printf("  | ID   | NOME                                  | CPF              | E-MAIL               | ENDEREÇO                                        | TELEFONE      |\n");
		printf("  +------+---------------------------------------+------------------+----------------------+-------------------------------------------------+---------------+\n");
		for (count = 1; count <= itemPorPagina; count++)
		{
			if (temp->proximo != 0)
			{


				/*if(((struct s_associado*)temp->dado)->deletado!=1)
				{*/
				//*************************************************************************************************
				printf("  | %-04d |", ((struct s_associado *)((struct s_noh*)temp->endereco)->dado)->id_associado);
				printf(" %-37s |", ((struct s_associado *)((struct s_noh*)temp->endereco)->dado)->nome);
				printf(" %-16s |", ((struct s_associado *)((struct s_noh*)temp->endereco)->dado)->cpf);
				printf(" %-20s |", ((struct s_associado *)((struct s_noh*)temp->endereco)->dado)->email);
				printf(" %-47s |", ((struct s_associado *)((struct s_noh*)temp->endereco)->dado)->endereco);
				printf(" %-13s |", ((struct s_associado *)((struct s_noh*)temp->endereco)->dado)->telefone);
				printf(" \n");
				/*		} else
				count--;*/

				temp = temp->proximo;


			}
			else
				printf("  |      |                                       |                  |                      |                                                 |               |\n");

		}
		printf("  +------+---------------------------------------+------------------+----------------------+-------------------------------------------------+---------------+\n");
		printf("\n");
		printf(" [0=Sair][1=Página anterior][2=Próxima página]    [página %d de %d]    [%d associado(s)]\n", pagAtual, pagTotal, itemTotal);

		while (op != 0)
		{
			op = getNumero();
			if (op == 0)
				break;

			if (op == 1)
			{
				if (pagAtual>1)
				{
					int aux, v;


					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) != 0)) {
						//printf("1");
						v = itemPorPagina + (itemTotal - ((pagAtual - 1)*itemPorPagina));
					}

					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) == 0))
					{
						//printf("2");
						v = (2 * itemPorPagina);
					}

					if ((pagAtual<pagTotal)) {
						//printf("3");
						v = (2 * itemPorPagina);
					}

					///printf(" [%d]\n",v);
					//congelarTela();

					for (aux = 1; aux <= v; aux++)
					{
						temp = temp->anterior;
					}
					pagAtual--;
					limparTela();
					break;
				}
			}

			if (op == 2)
			{
				if (pagAtual<pagTotal)
				{
					pagAtual++;
					limparTela();
					break;
				}
			}
		}

	}

	system("mode con lines=60");
	system("mode con cols=62");
}
int cmpStructNohOrd(const void *a, const void *b)
{
	struct s_noh_ord *ia = (struct s_noh_ord *)a;
	struct s_noh_ord *ib = (struct s_noh_ord *)b;
	return strcmp(ia->nome, ib->nome);
}
int tamString(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++);

	return i + 1;
}
void QuickSortAssociado()
{
	int qnt, i;
	struct s_noh *temp;
	struct s_noh_ord *tempO, *tempO2;

	// vetor de ponteiros para noh
	struct s_noh_ord *vetor;

	// conta quantos nomes há
	for (qnt = 0, temp = listaA; temp->proximo != 0; temp = temp->proximo)
		if (((struct s_associado*)temp->dado)->deletado != 1)
			qnt++;

	// aloca memoria para QNT noh's
	vetor = (struct s_noh_ord *) calloc(sizeof(struct s_noh_ord), qnt);

	// copia os nomes para os noh's
	for (i = 0, temp = listaA; temp->proximo != 0; temp = temp->proximo, i++)
	{
		if (((struct s_associado*)temp->dado)->deletado != 1)
		{
			vetor[i].endereco = temp;
			vetor[i].nome = (char *)calloc(tamString((char*)&((struct s_associado*)temp->dado)->nome), sizeof(char));
			strcpy(vetor[i].nome, ((struct s_associado*)temp->dado)->nome);
			vetor[i].proximo = 0;
		}
		else
			i--;
	}

	// qsort neles
	qsort(vetor, qnt, sizeof(struct s_noh_ord), cmpStructNohOrd);

	// cria a lista encadeada
	for (tempO = listaAOrd, i = 0; i<qnt; i++)
	{
		tempO->nome = (char *)calloc(tamString(vetor[i].nome), sizeof(char));
		tempO->endereco = vetor[i].endereco;
		tempO->proximo = vetor[i].proximo;
		tempO->proximo = (struct s_noh_ord *) calloc(sizeof(struct s_noh_ord), 1);

		tempO2 = tempO;

		tempO = tempO->proximo;

		tempO->anterior = tempO2;
		tempO->proximo = 0;
		tempO->nome = 0;
		tempO->endereco = 0;
	}

	// libera a memoria
	//for(i=0;i<qnt;i++)
	//free((struct s_noh_ord *)&vetor[i]);
}
struct s_noh* getNohAssociado()
{
	struct s_noh *temp;

	int id_associado;

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");

	for (temp = listaA; temp->proximo != 0; temp = temp->proximo)
		if (id_associado == ((struct s_associado*)temp->dado)->id_associado)
			return temp;

	if (temp == 0 || (((struct s_associado*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return 0;
	}

	return 0;
}
void exibirDadosAssociado(struct s_associado *temp)
{
	printf("   Nome......: %s\n", ((struct s_associado*)temp)->nome);
	printf("   CPF.......: %s\n", ((struct s_associado*)temp)->cpf);
	printf("   E-mail....: %s\n", ((struct s_associado*)temp)->email);
	printf("   Endereço..: %s\n", ((struct s_associado*)temp)->endereco);
	printf("   Telefone..: %s\n", ((struct s_associado*)temp)->telefone);
	printf("\n");
}
// *************************************************************************************************************************************
// *
// * LIVRO
// *
// *************************************************************************************************************************************
void janelaCadastrarLivro()
{
	/*
	short tipo;
	short deletado;

	int id_livro;
	char titulo[_STRTIT];
	char subtitulo[_STRSUBTIT];
	char autor[_STRAUT];
	char isbn[_STRISBN];
	char editora[_STREDITORA];
	char edicao[_STREDICAO];
	int exemplar;
	int qnt_livro_emprestado;
	reservadoParaIdAssociado;
	*/

	struct s_noh *temp;
	struct s_livro *livro;
	livro = (struct s_livro*) calloc(sizeof(struct s_livro), 1);

	livro->tipo = _LIVRO;
	livro->deletado = 0;
	livro->id_livro = getNovaIdLivro();
	livro->qnt_livro_emprestado = 0;
	livro->reservadoParaIdAssociado = 0;

	// TOPO
	limparTela();
	printf("\n");
	mostrarTituloPequeno("Cadastro de livro");
	printf("\n");

	// PERGUNTAS
	printf("  ID: %d", livro->id_livro);
	printf("\n");

	printf("  Digite o ISBN do livro:\n  > ");
	strcpy(livro->isbn, getString(_STRISBN, _F_NUMERO));
	printf("\n");

	for (temp = listaL; temp->proximo != 0; temp = temp->proximo) {
		if ((strcmp(livro->isbn, ((struct s_livro*)temp->dado)->isbn)) == 0)
		{
			printf("LIVRO JA CADASTRADO\n\n"); congelarTela(); return;
		}
	}
	printf("  Digite o titulo do livro:\n  > ");
	strcpy(livro->titulo, getString(_STRTIT, _F_LETRAESPACO));
	printf("\n");

	printf("  Digite o subtítulo do livro:\n  > ");
	strcpy(livro->subtitulo, getString(_STRSUBTIT, _F_LETRAESPACO));
	printf("\n");

	printf("  Digite o autor do livro:\n  > ");
	strcpy(livro->autor, getString(_STRAUT, _F_LETRAESPACO));
	printf("\n");

	printf("  Digite a editora do livro:\n  > ");
	strcpy(livro->editora, getString(_STREDITORA, _F_LETRAESPACO));
	printf("\n");

	printf("  Digite a edição do livro:\n  > ");
	strcpy(livro->edicao, getString(_STREDICAO, _F_LETRAESPACO));
	printf("\n");

	printf("  Digite a quantidade de exemplares do livro:\n  > ");
	scanf("%d", &livro->exemplar);
	printf("\n");

	// CONFIRMAÇÃO
	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Cadastro de livro");
		printf("\n");

		printf("   Titulo......: %s\n", livro->titulo);
		printf("   Subtítulo.......: %s\n", livro->subtitulo);
		printf("   Autor....: %s\n", livro->autor);
		printf("   ISBN..: %s\n", livro->isbn);
		printf("   Editora..: %s\n", livro->editora);
		printf("   Edição..: %s\n", livro->edicao);
		printf("   Exemplar(es)..: %d\n", livro->exemplar);
		printf("\n");
		printf(" [0=Cancelar] [1=Cadastrar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			adicionarNoh(0, livro, 0);
			return;
		}
	}
}
void janelaEditarLivro()
{
	struct s_noh *temp;

	int id_livro;

	struct s_livro *livro;
	livro = (struct s_livro*) calloc(sizeof(struct s_livro), 1);

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Editar livro");
	printf("\n");

	printf("  Informe a identificação do livro:\n  > ");
	scanf("%d", &id_livro);
	printf("\n");

	temp = getNohLivroByIdLivro(id_livro);

	if (temp == 0 || (((struct s_livro*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Livro não encontrado\n");
		congelarTela();
		return;
	}

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Editar livro");
	printf("\n");

	printf("   Titulo......: %s\n", ((struct s_livro*)temp->dado)->titulo);
	printf("   Subtítulo.......: %s\n", ((struct s_livro*)temp->dado)->subtitulo);
	printf("   Autor....: %s\n", ((struct s_livro*)temp->dado)->autor);
	printf("   ISBN..: %s\n", ((struct s_livro*)temp->dado)->isbn);
	printf("   Editora..: %s\n", ((struct s_livro*)temp->dado)->editora);
	printf("   Edição..: %s\n", ((struct s_livro*)temp->dado)->edicao);
	printf("   Exemplar(es)..: %d\n", ((struct s_livro*)temp->dado)->exemplar);
	printf("\n");
	printf(" [0=Cancelar] [1=Continuar]\n");

	if (getNumero() == 0)
	{
		limparTela();
		printf("\n  Cancelado\n");
		congelarTela();
		return;
	}

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Editar livro");
	printf("\n");

	printf("   Titulo......: %s\n", ((struct s_livro*)temp->dado)->titulo);
	printf("   Subtítulo.......: %s\n", ((struct s_livro*)temp->dado)->subtitulo);
	printf("   Autor....: %s\n", ((struct s_livro*)temp->dado)->autor);
	printf("   ISBN..: %s\n", ((struct s_livro*)temp->dado)->isbn);
	printf("   Editora..: %s\n", ((struct s_livro*)temp->dado)->editora);
	printf("   Edição..: %s\n", ((struct s_livro*)temp->dado)->edicao);
	printf("   Exemplar(es)..: %d\n", ((struct s_livro*)temp->dado)->exemplar);
	printf("\n");

	printf("  ID: %d", livro->id_livro);
	printf("\n");

	printf("  Digite o titulo do livro:\n  > ");
	strcpy(livro->titulo, getString(_STRTIT, _F_LETRAESPACO));
	if (strcmp(livro->titulo, "") == 0)
	{
		strcpy(livro->titulo, ((struct s_livro*)temp->dado)->titulo);
		printf("  > %s\n", livro->titulo);
	}
	printf("\n");

	printf("  Digite o subtítulo do livro:\n  > ");
	strcpy(livro->subtitulo, getString(_STRSUBTIT, _F_LETRAESPACO));
	if (strcmp(livro->subtitulo, "") == 0)
	{
		strcpy(livro->subtitulo, ((struct s_livro*)temp->dado)->subtitulo);
		printf("  > %s\n", livro->subtitulo);
	}
	printf("\n");

	printf("  Digite o autor do livro:\n  > ");
	strcpy(livro->autor, getString(_STRAUT, _F_LETRAESPACO));
	if (strcmp(livro->autor, "") == 0)
	{
		strcpy(livro->autor, ((struct s_livro*)temp->dado)->autor);
		printf("  > %s\n", livro->autor);
	}
	printf("\n");

	printf("  Digite o ISBN do livro:\n  > ");
	strcpy(livro->isbn, getString(_STRISBN, _F_LETRAESPACO));
	if (strcmp(livro->isbn, "") == 0)
	{
		strcpy(livro->isbn, ((struct s_livro*)temp->dado)->isbn);
		printf("  > %s\n", livro->isbn);
	}
	printf("\n");

	printf("  Digite a editora do livro:\n  > ");
	strcpy(livro->editora, getString(_STREDITORA, _F_LETRAESPACO));
	if (strcmp(livro->editora, "") == 0)
	{
		strcpy(livro->editora, ((struct s_livro*)temp->dado)->editora);
		printf("  > %s\n", livro->editora);
	}
	printf("\n");

	printf("  Digite a edição do livro:\n  > ");
	strcpy(livro->edicao, getString(_STREDICAO, _F_LETRAESPACO));
	if (strcmp(livro->edicao, "") == 0)
	{
		strcpy(livro->edicao, ((struct s_livro*)temp->dado)->edicao);
		printf("  > %s\n", livro->edicao);
	}
	printf("\n");

	printf("  Digite a quantidade de exemplares do livro:\n  > ");
	scanf("%d", &livro->exemplar);
	printf("\n");

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Editar livro");
		printf("\n");

		printf("   Dados antigos:\n");
		printf("   Titulo......: %s\n", ((struct s_livro*)temp->dado)->titulo);
		printf("   Subtítulo.......: %s\n", ((struct s_livro*)temp->dado)->subtitulo);
		printf("   Autor....: %s\n", ((struct s_livro*)temp->dado)->autor);
		printf("   ISBN..: %s\n", ((struct s_livro*)temp->dado)->isbn);
		printf("   Editora..: %s\n", ((struct s_livro*)temp->dado)->editora);
		printf("   Edição..: %s\n", ((struct s_livro*)temp->dado)->edicao);
		printf("   Exemplar(es)..: %d\n", ((struct s_livro*)temp->dado)->exemplar);
		printf("\n");

		printf("   Dados modificados:\n");
		printf("   Titulo......: %s\n", livro->titulo);
		printf("   Subtítulo.......: %s\n", livro->subtitulo);
		printf("   Autor....: %s\n", livro->autor);
		printf("   ISBN..: %s\n", livro->isbn);
		printf("   Editora..: %s\n", livro->editora);
		printf("   Edição..: %s\n", livro->edicao);
		printf("   Exemplar(es)..: %d\n", livro->exemplar);
		printf("\n");
		printf(" [0=Cancelar] [1=Editar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			strcpy(((struct s_livro*)temp->dado)->titulo, livro->titulo);
			strcpy(((struct s_livro*)temp->dado)->subtitulo, livro->subtitulo);
			strcpy(((struct s_livro*)temp->dado)->autor, livro->autor);
			strcpy(((struct s_livro*)temp->dado)->isbn, livro->isbn);
			strcpy(((struct s_livro*)temp->dado)->editora, livro->editora);
			strcpy(((struct s_livro*)temp->dado)->edicao, livro->edicao);
			((struct s_livro*)temp->dado)->exemplar = livro->exemplar;
			temp->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void janelaRemoverLivro()
{
	struct s_noh *temp;

	int id_livro;

	struct s_livro *livro;
	livro = (struct s_livro*) calloc(sizeof(struct s_livro), 1);

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Remover livro");
	printf("\n");

	printf("  Informe a identificação do livro:\n  > ");
	scanf("%d", &id_livro);
	printf("\n");

	temp = getNohLivroByIdLivro(id_livro);

	if (temp == 0 || (((struct s_livro*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Livro não encontrado\n");
		congelarTela();
		return;
	}

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Remover livro");
		printf("\n");

		printf("   Titulo......: %s\n", ((struct s_livro*)temp->dado)->titulo);
		printf("   Subtítulo.......: %s\n", ((struct s_livro*)temp->dado)->subtitulo);
		printf("   Autor....: %s\n", ((struct s_livro*)temp->dado)->autor);
		printf("   ISBN..: %s\n", ((struct s_livro*)temp->dado)->isbn);
		printf("   Editora..: %s\n", ((struct s_livro*)temp->dado)->editora);
		printf("   Edição..: %s\n", ((struct s_livro*)temp->dado)->edicao);
		printf("   Exemplar(es)..: %d\n", ((struct s_livro*)temp->dado)->exemplar);
		printf("\n");
		printf(" [0=Cancelar] [1=Remover]\n");

		if (getNumero() == 0)
		{
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		}
		if (getNumero() == 1)
		{
			((struct s_livro*)temp->dado)->deletado = 1;
			temp->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void janelaListarLivro()
{
	struct s_noh *temp;

	int itemTotal = 0, pagTotal = 0, pagAtual = 1, op = (-1), count, itemPorPagina = 40;

	// Verifica se existe registros para mostrar
	temp = listaL;
	if (temp->proximo == 0)
	{
		limparTela();
		printf("Não foram encontrados registros\n");
		congelarTela();
		return;
	}

	// Descobre quantos itens existem na lista encadeada (1 item = 1)
	for (temp = listaL, itemTotal = 0; temp->proximo != 0; temp = temp->proximo, itemTotal++)
		if (((struct s_livro*)temp->dado)->deletado == 1)
			itemTotal--;

	// Calcula o número de páginas necessárias
	pagTotal = itemTotal / itemPorPagina;
	if ((itemTotal%itemPorPagina) != 0)
		pagTotal++;

	// Ajusta a tela
	system("mode con lines=54");
	system("mode con cols=160");

	temp = listaL;


	while (op != 0)
	{

		printf("\n");
		mostrarTituloGrande("Lista de livros");
		printf("\n");
		printf("  +------+---------------------------------------------------------------------------------------------------------------------------------------------------+\n");
		printf("  | ID   | TITULO                                | SUBTITULO                 | AUTOR                         | EDITORA             | EXP  | ISBN             |\n");
		printf("  +------+---------------------------------------------------------------------------------------------------------------------------------------------------+\n");
		for (count = 1; count <= itemPorPagina; count++)
		{
			if (temp->proximo != 0)
			{


				if (((struct s_livro*)temp->dado)->deletado != 1)
				{
					printf("  | %-04d |", ((struct s_livro*)temp->dado)->id_livro);
					printf(" %-37s |", ((struct s_livro*)temp->dado)->titulo);
					printf(" %-25s |", ((struct s_livro*)temp->dado)->subtitulo);
					printf(" %-29s |", ((struct s_livro*)temp->dado)->autor);
					printf(" %-19s |", ((struct s_livro*)temp->dado)->editora);
					printf(" %-4d |", ((struct s_livro*)temp->dado)->exemplar);
					printf(" %-16s |", ((struct s_livro*)temp->dado)->isbn);
					printf(" \n");
				}
				else
					count--;

				temp = temp->proximo;


			}
			else
				printf("  |      |                                       |                           |                               |                     |      |                  |\n");

		}
		printf("  +----------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
		printf("\n");
		printf(" [0=Sair][1=Página anterior][2=Próxima página]    [página %d de %d]    [%d livro(s)]\n", pagAtual, pagTotal, itemTotal);

		while (op != 0)
		{
			op = getNumero();
			if (op == 0)
				break;

			if (op == 1)
			{
				if (pagAtual>1)
				{
					int aux, v;


					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) != 0)) {
						//printf("1");
						v = itemPorPagina + (itemTotal - ((pagAtual - 1)*itemPorPagina));
					}

					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) == 0))
					{
						//printf("2");
						v = (2 * itemPorPagina);
					}

					if ((pagAtual<pagTotal)) {
						//printf("3");
						v = (2 * itemPorPagina);
					}

					///printf(" [%d]\n",v);
					//congelarTela();

					for (aux = 1; aux <= v; aux++)
					{
						temp = temp->anterior;
					}
					pagAtual--;
					limparTela();
					break;
				}
			}

			if (op == 2)
			{
				if (pagAtual<pagTotal)
				{
					pagAtual++;
					limparTela();
					break;
				}
			}
		}

	}

	system("mode con lines=60");
	system("mode con cols=62");
}
void janelaBuscarLivro()
{
}
int getNovaIdLivro()
{
	int id = 1; struct s_noh *temp;

	for (temp = listaL; temp->proximo != 0; temp = temp->proximo, id++);

	for (temp = listaL; temp->proximo != 0; temp = temp->proximo)
		if (id == ((struct s_livro*)temp->dado)->id_livro)
		{
			id++;
			temp = listaL;
		}

	return id;
}
struct s_noh * getNohLivro()
{
	struct s_noh *temp; int id_livro;

	printf("  Informe a identificação do livro:\n  > ");
	scanf("%d", &id_livro);
	printf("\n");

	for (temp = listaL; temp->proximo != 0; temp = temp->proximo)
		if (id_livro == ((struct s_livro*)temp->dado)->id_livro)
			return temp;

	if (temp == 0 || (((struct s_associado*)temp->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Livro não encontrado\n");
		congelarTela();
		return 0;
	}

	return 0;
}
void reservarLivro()
{
	int id_associado, id_livro;
	struct s_noh *nohAssociado, *nohLivro;


	// TOPO
	limparTela();
	printf("\n");
	mostrarTituloPequeno("Reservar de livro");
	printf("\n");

	printf("  Informe a identificação do associado:\n  > ");
	scanf("%d", &id_associado);
	printf("\n");
	nohAssociado = getNohAssociadoByIdAssociado(id_associado);
	if (nohAssociado == 0 || (((struct s_associado*)nohAssociado->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Associado não encontrado\n");
		congelarTela();
		return;
	}

	printf("   Nome......: %s\n", ((struct s_associado*)nohAssociado->dado)->nome);
	printf("   CPF.......: %s\n", ((struct s_associado*)nohAssociado->dado)->cpf);
	printf("   E-mail....: %s\n", ((struct s_associado*)nohAssociado->dado)->email);
	printf("   Endereço..: %s\n", ((struct s_associado*)nohAssociado->dado)->endereco);
	printf("   Telefone..: %s\n", ((struct s_associado*)nohAssociado->dado)->telefone);
	printf("\n");

	printf("  Informe a identificação do livro:\n  > ");
	scanf("%d", &id_livro);
	printf("\n");
	nohLivro = getNohLivroByIdLivro(id_livro);
	if (nohLivro == 0 || (((struct s_associado*)nohLivro->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Livro não encontrado\n");
		congelarTela();
		return;
	}

	if (((struct s_livro*)nohLivro->dado)->qnt_livro_emprestado >= ((struct s_livro*)nohLivro->dado)->exemplar)
	{
		limparTela();
		printf("\n  Não há exemplares disponíveis.\nCancelado.\n");
		congelarTela();
		return;
	}

	if (((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado != 0 && (((struct s_livro*)nohLivro->dado)->exemplar - ((struct s_livro*)nohLivro->dado)->qnt_livro_emprestado) < 2)
	{
		if (((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado != ((struct s_associado*)nohAssociado)->id_associado)
		{
			limparTela();
			printf("\n  O livro está reservado.\n");
			congelarTela();
			return;
		}
	}

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Reserva de livro");
	printf("\n");

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Empréstimo de livro");
		printf("\n");
		printf("   Nome......: %s\n", ((struct s_associado*)nohAssociado->dado)->nome);
		printf("   CPF.......: %s\n", ((struct s_associado*)nohAssociado->dado)->cpf);
		printf("   E-mail....: %s\n", ((struct s_associado*)nohAssociado->dado)->email);
		printf("   Endereço..: %s\n", ((struct s_associado*)nohAssociado->dado)->endereco);
		printf("   Telefone..: %s\n", ((struct s_associado*)nohAssociado->dado)->telefone);
		printf("\n");

		printf("   Titulo..........: %s\n", ((struct s_livro*)nohLivro->dado)->titulo);
		printf("   Subtítulo.......: %s\n", ((struct s_livro*)nohLivro->dado)->subtitulo);
		printf("   Autor...........: %s\n", ((struct s_livro*)nohLivro->dado)->autor);
		printf("   ISBN............: %s\n", ((struct s_livro*)nohLivro->dado)->isbn);
		printf("   Editora.........: %s\n", ((struct s_livro*)nohLivro->dado)->editora);
		printf("   Edição..........: %s\n", ((struct s_livro*)nohLivro->dado)->edicao);
		printf("   Exemplar(es)....: %d\n", ((struct s_livro*)nohLivro->dado)->exemplar);
		printf("\n");
		printf("   Data de empréstimo: %d/%d/%d\n", getDia(), getMes(), getAno());
		printf("\n");
		printf(" [0=Cancelar] [1=Reservar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado = ((struct s_associado*)nohAssociado->dado)->id_associado;

			nohLivro->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void tirarReserva()
{

	int id_associado, id_livro;
	struct s_noh *nohAssociado, *nohLivro;


	// TOPO
	limparTela();
	printf("\n");
	mostrarTituloPequeno("Tirar reserva de livro");
	printf("\n");

	printf("  Informe a identificação do livro:\n  > ");
	scanf("%d", &id_livro);
	printf("\n");
	nohLivro = getNohLivroByIdLivro(id_livro);
	if (nohLivro == 0 || (((struct s_associado*)nohLivro->dado)->deletado == 1))
	{
		limparTela();
		printf("\n  Livro não encontrado\n");
		congelarTela();
		return;
	}

	if (((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado != 0)
	{
		((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado = 0;
	}
	else {
		limparTela();
		printf("\n  Livro nao tem reserva\n");
		congelarTela();
		return;
	}


	limparTela();
	printf("\n");
	mostrarTituloPequeno("Tirar Reserva de livro");
	printf("\n");

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Tirar reserva de livro");
		printf("\n");

		printf("   Titulo..........: %s\n", ((struct s_livro*)nohLivro->dado)->titulo);
		printf("   Subtítulo.......: %s\n", ((struct s_livro*)nohLivro->dado)->subtitulo);
		printf("   Autor...........: %s\n", ((struct s_livro*)nohLivro->dado)->autor);
		printf("   ISBN............: %s\n", ((struct s_livro*)nohLivro->dado)->isbn);
		printf("   Editora.........: %s\n", ((struct s_livro*)nohLivro->dado)->editora);
		printf("   Edição..........: %s\n", ((struct s_livro*)nohLivro->dado)->edicao);
		printf("   Exemplar(es)....: %d\n", ((struct s_livro*)nohLivro->dado)->exemplar);
		printf("\n");
		printf(" [0=Cancelar] [1=Tirar reserva]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			nohLivro->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void exibirDadoLivro(struct s_livro *temp)
{
	printf("   Titulo..........: %s\n", ((struct s_livro*)temp)->titulo);
	printf("   Subtítulo.......: %s\n", ((struct s_livro*)temp)->subtitulo);
	printf("   Autor...........: %s\n", ((struct s_livro*)temp)->autor);
	printf("   ISBN............: %s\n", ((struct s_livro*)temp)->isbn);
	printf("   Editora.........: %s\n", ((struct s_livro*)temp)->editora);
	printf("   Edição..........: %s\n", ((struct s_livro*)temp)->edicao);
	printf("   Exemplar(es)....: %d\n", ((struct s_livro*)temp)->exemplar);
}
// *************************************************************************************************************************************
// *
// * EMPRESTIMO
// *
// *************************************************************************************************************************************
void janelaEmprestarLivro()
{
	/*
	short tipo;
	short deletado;
	int id_emprestimo;
	struct s_associado *associado;
	struct s_livro *livro[3];
	int dataEmprestado[3];
	int dataDevolvido[3];
	*/

	// -*****************************************
	int id_associado = 0;
	int id_livro = 0;

	struct s_noh *nohAssociado, *nohLivro;

	// Cria a variável da estrutura
	struct s_emprestimo *emprestimo;
	// Aloca memória para a estrutura
	emprestimo = (struct s_emprestimo*) calloc(sizeof(struct s_emprestimo), 1);
	// Zera alguns dados da estrutura
	emprestimo->tipo = _EMPRESTIMO;
	emprestimo->deletado = 0;
	emprestimo->id_emprestimo = getNovaIdEmprestimo();

	// TOPO
	limparTela();
	mostrarTituloPequeno("Empréstimo de livro");

	printf("  ID: %d", emprestimo->id_emprestimo);
	printf("\n");

	nohAssociado = getNohAssociado();

	if (((struct s_associado*)nohAssociado->dado)->qnt_livro_emprestado >= 3)
	{
		limparTela();
		printf("\n  Associado já possui 3 exemplares emprestados.\nCancelado.\n");
		congelarTela();
		return;
	}

	if (((struct s_associado*)nohAssociado->dado)->bloqueado == 1)
	{
		int diasBloqueado = diasEntreDuasDatas(getDia(), getMes() - 1, getAno(), ((struct s_associado*)nohAssociado->dado)->dia_do_bloqueio[0], ((struct s_associado*)nohAssociado->dado)->dia_do_bloqueio[1] - 1, ((struct s_associado*)nohAssociado->dado)->dia_do_bloqueio[2]);
		if (diasBloqueado>180)
		{
			printf("\n  Usuário foi desbloqueado!\n  Já se passou 180 dias desde o bloqueio!\n");
			((struct s_associado*)nohAssociado->dado)->bloqueado = 0;
			((struct s_associado*)nohAssociado->dado)->dia_do_bloqueio[0] = 0;
			((struct s_associado*)nohAssociado->dado)->dia_do_bloqueio[1] = 0;
			((struct s_associado*)nohAssociado->dado)->dia_do_bloqueio[2] = 0;
			((struct s_associado*)nohAssociado->dado)->qnt_de_multa = 0;
		}
		else
		{
			limparTela();
			printf("\n  Associado bloqueado!  Dias bloqueado: %d\n  Dias para acabar o bloqueio: %d\n  Cancelado.\n", diasBloqueado, 180 - diasBloqueado);
			congelarTela();
			return;
		}
	}

	exibirDadosAssociado(nohAssociado);

	nohLivro = getNohLivro();

	if (((struct s_livro*)nohLivro->dado)->qnt_livro_emprestado >= ((struct s_livro*)nohLivro->dado)->exemplar)
	{
		limparTela();
		printf("\n  Não há exemplares disponíveis.\nCancelado.\n");
		congelarTela();
		return;
	}

	if (((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado != 0 && (((struct s_livro*)nohLivro->dado)->exemplar - ((struct s_livro*)nohLivro->dado)->qnt_livro_emprestado) < 2)
	{
		if (((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado != ((struct s_associado*)nohAssociado->dado)->id_associado)
		{
			limparTela();
			printf("%d  %d\n", ((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado, ((struct s_associado*)nohAssociado->dado)->id_associado);
			printf("\n  O livro está reservado.\n");
			congelarTela();
			return;
		}
	}

	while (1)
	{

		mostrarTituloPequeno("Empréstimo de livro");

		exibirDadosAssociado(nohAssociado);
		exibirDadoLivro(nohLivro);

		printf("\n");
		printf("   Data de empréstimo: %d/%d/%d\n", getDia(), getMes(), getAno());
		printf("\n");

		printf(" [0=Cancelar] [1=Confirmar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			((struct s_associado*)nohAssociado->dado)->qnt_livro_emprestado += 1;
			((struct s_livro*)nohLivro->dado)->qnt_livro_emprestado += 1;
			((struct s_livro*)nohLivro->dado)->reservadoParaIdAssociado = 0;

			emprestimo->id_associado = id_associado;
			emprestimo->id_livro = id_livro;
			emprestimo->dataEmprestado[0] = getDia();
			emprestimo->dataEmprestado[1] = getMes();
			emprestimo->dataEmprestado[2] = getAno();
			nohAssociado->modificado = 1;
			nohLivro->modificado = 1;
			adicionarNoh(0, 0, emprestimo);
			return;
		}
	}
}
void janelaDevolverLivro()
{
	int id_associado = 0, id_livro = 0, id_livros[3] = { 0,0,0 }, i;
	struct s_noh *nohEmprestimo, *nohAssociado, *nohLivro, *nohEmprestimos[3] = { 0,0,0 };

	mostrarTituloPequeno("Devolução de livro");

	nohAssociado = getNohAssociado();


	for (nohEmprestimo = listaE, i = 0; nohEmprestimo->proximo != 0 && i <= 3; nohEmprestimo = nohEmprestimo->proximo)
		if (id_associado == ((struct s_emprestimo*)nohEmprestimo->dado)->id_associado)
		{
			id_livro = ((struct s_emprestimo*)nohEmprestimo->dado)->id_livro;
			printf(" [%d] %s\n", i, ((struct s_livro*) getNohLivroByIdLivro(id_livro)->dado)->titulo);
			id_livros[i] = id_livro;
			nohEmprestimos[i] = nohEmprestimo;
			i++;
		}

	printf("\n");
	printf("  Informe o livro a ser devolvido:\n  > ");
	i = getNumero();
	if (i != 0 && i != 1 && i != 2)
	{
		limparTela();
		printf("\n  Opção inválida.\n  Cancelado\n");
		congelarTela();
		return;
	}
	id_livro = id_livros[i];
	printf("\n");
	nohLivro = getNohLivroByIdLivro(id_livro);
	if (nohLivro == 0)
	{
		limparTela();
		printf("\n  Livro não encontrado\n");
		congelarTela();
		return;
	}

	limparTela();
	printf("\n");
	mostrarTituloPequeno("Devolução de livro");
	printf("\n");

	nohAssociado = getNohAssociadoByIdAssociado(id_associado);

	int dia = getDia();
	int mes = getMes();
	int ano = getAno();

	int diaD = ((struct s_emprestimo *)nohEmprestimos[i]->dado)->dataEmprestado[0];
	int mesD = ((struct s_emprestimo *)nohEmprestimos[i]->dado)->dataEmprestado[1];
	int anoD = ((struct s_emprestimo *)nohEmprestimos[i]->dado)->dataEmprestado[2];

	((struct s_emprestimo *)nohEmprestimos[i]->dado)->dataDevolvido[0] = dia;
	((struct s_emprestimo *)nohEmprestimos[i]->dado)->dataDevolvido[1] = mes;
	((struct s_emprestimo *)nohEmprestimos[i]->dado)->dataDevolvido[2] = ano;

	int diasComLivro = diasEntreDuasDatas(diaD, mesD - 1, anoD, dia, mes - 1, ano);

	while (1)
	{
		limparTela();
		printf("\n");
		mostrarTituloPequeno("Devolução de livro");
		printf("\n");

		printf("   Dia emprestado: %d/%d/%d\n", diaD, mesD, anoD);
		printf("   Dia devolvido.: %d/%d/%d\n", dia, mes, ano);
		printf("   Dias com o livro: %d\n", diasComLivro);
		printf("\n");
		if (diasComLivro>3)
		{
			printf("   Multa: SIM \n");
			printf("   Valor da multa: %d reais \n", (diasComLivro - 3)*_VALORDAMULTA);

			((struct s_associado*) nohAssociado->dado)->qnt_de_multa += 1;
			((struct s_associado*) nohAssociado->dado)->multa += ((diasComLivro - 3)*_VALORDAMULTA);

			if (((struct s_associado*) nohAssociado->dado)->qnt_de_multa > 10)
			{
				((struct s_associado*) nohAssociado->dado)->bloqueado = 1;
				((struct s_associado*) nohAssociado->dado)->dia_do_bloqueio[0] = dia;
				((struct s_associado*) nohAssociado->dado)->dia_do_bloqueio[1] = mes;
				((struct s_associado*) nohAssociado->dado)->dia_do_bloqueio[2] = ano;
			}

		}
		else
		{
			printf("   Multa: NÃO \n");
		}
		printf("\n");
		printf("   Titulo......: %s\n", ((struct s_livro*)nohLivro->dado)->titulo);
		printf("   Subtítulo.......: %s\n", ((struct s_livro*)nohLivro->dado)->subtitulo);
		printf("   Autor....: %s\n", ((struct s_livro*)nohLivro->dado)->autor);
		printf("   ISBN..: %s\n", ((struct s_livro*)nohLivro->dado)->isbn);
		printf("   Editora..: %s\n", ((struct s_livro*)nohLivro->dado)->editora);
		printf("   Edição..: %s\n", ((struct s_livro*)nohLivro->dado)->edicao);
		printf("   Exemplar(es)..: %d\n", ((struct s_livro*)nohLivro->dado)->exemplar);
		printf("\n");
		printf(" [0=Cancelar] [1=Confirmar]\n");

		switch (getNumero())
		{
		case 0:
			limparTela();
			printf("\n  Cancelado\n");
			congelarTela();
			return;
		case 1:
			((struct s_associado*) nohAssociado->dado)->qnt_livro_emprestado -= 1;
			((struct s_livro*) nohLivro->dado)->qnt_livro_emprestado -= 1;
			((struct s_emprestimo *)nohEmprestimos[i]->dado)->deletado = 1;
			nohEmprestimos[i]->modificado = 1;
			nohLivro->modificado = 1;
			nohAssociado->modificado = 1;
			gravarDisco();
			return;
		}
	}
}
void janelaListarEmprestimo()
{
	//*************************************************************************************************
	struct s_noh *temp, *nohAssociado, *nohLivro;
	int itemTotal = 0, pagTotal = 0, pagAtual = 1, op = (-1), count, itemPorPagina = 40;

	temp = listaE;
	if (temp->proximo == 0)
	{
		limparTela();
		printf("Não foram encontrados registros\n");
		congelarTela();
		return;
	}

	for (temp = listaE, itemTotal = 0; temp->proximo != 0; temp = temp->proximo, itemTotal++)
		if (((struct s_emprestimo*)temp->dado)->deletado == 1)
			itemTotal--;

	pagTotal = itemTotal / itemPorPagina;
	if ((itemTotal%itemPorPagina) != 0)
		pagTotal++;

	system("mode con lines=54");
	system("mode con cols=160");

	temp = listaE;



	while (op != 0)
	{

		printf("\n");
		mostrarTituloGrande("Lista de empréstimos");
		printf("\n");
		printf("  +------+---------------------------------------+------------------+----------------------+-----------------------------------------------------------------+\n");
		printf("  | ID   | NOME                                  | CPF              | QTD EMPRESTADA       | TITULO DO LIVRO                                                 |\n");
		printf("  +------+---------------------------------------+------------------+----------------------+-----------------------------------------------------------------+\n");
		for (count = 1; count <= itemPorPagina; count++)
		{
			if (temp->proximo != 0)
			{
				if (((struct s_emprestimo*)temp->dado)->deletado != 1)
				{
					nohAssociado = getNohAssociadoByIdAssociado(((struct s_emprestimo*)temp->dado)->id_associado);
					nohLivro = getNohLivroByIdLivro(((struct s_emprestimo*)temp->dado)->id_livro);
					if (nohLivro == 0 || nohAssociado == 0)
						return;
					printf("  | %-04d |", ((struct s_emprestimo*)temp->dado)->id_emprestimo);
					printf(" %-37s |", ((struct s_associado*)nohAssociado->dado)->nome);
					printf(" %-16s |", ((struct s_associado*)nohAssociado->dado)->cpf);
					printf(" %-20d |", ((struct s_associado*)nohAssociado->dado)->qnt_livro_emprestado);
					printf(" %-63s |", ((struct s_livro*)nohLivro->dado)->titulo);
					printf(" \n");
				}
				else
					count--;

				temp = temp->proximo;

			}
			else
				printf("  |      |                                       |                  |                      |                                                                 |\n");

		}
		printf("  +------+---------------------------------------+------------------+----------------------+-----------------------------------------------------------------+\n");
		printf("\n");
		printf(" [0=Sair][1=Página anterior][2=Próxima página]    [página %d de %d]    [%d empréstimo(s)]\n", pagAtual, pagTotal, itemTotal);

		while (op != 0)
		{
			op = getNumero();

			if (op == 0)
				break;
			if (op == 1)
			{
				if (pagAtual>1)
				{
					int aux, v;

					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) != 0))
						v = itemPorPagina + (itemTotal - ((pagAtual - 1)*itemPorPagina));
					if ((pagAtual == pagTotal) && ((itemTotal % ((pagTotal - 1)*itemPorPagina)) == 0))
						v = (2 * itemPorPagina);
					if ((pagAtual<pagTotal))
						v = (2 * itemPorPagina);
					for (aux = 1; aux <= v; aux++)
						temp = temp->anterior;
					pagAtual--;
					limparTela();
					break;
				}
			}
			if (op == 2)
				if (pagAtual<pagTotal)
				{
					pagAtual++;
					limparTela();
					break;
				}
		}

	}
	system("mode con lines=60");
	system("mode con cols=62");
}
int getNovaIdEmprestimo()
{
	int id = 1; struct s_noh *temp;

	for (temp = listaE; temp->proximo != 0; temp = temp->proximo, id++);

	for (temp = listaE; temp->proximo != 0; temp = temp->proximo)
		if (id == ((struct s_emprestimo*)temp->dado)->id_emprestimo)
		{
			id++;
			temp = listaE;
		}

	return id;
}
struct s_noh * getNohEmprestimoByIdAssociado(int id_associado)
{
	struct s_noh *temp;

	for (temp = listaE; temp->proximo != 0; temp = temp->proximo)
		if (id_associado == ((struct s_emprestimo*)temp->dado)->id_associado)
			return temp;

	return 0;
}
struct s_noh * getNohEmprestimoByIdLivro(int id_livro)
{
	struct s_noh *temp;

	for (temp = listaE; temp->proximo != 0; temp = temp->proximo)
		if (id_livro == ((struct s_emprestimo*)temp->dado)->id_livro)
		{
		}

	return 0;
}

struct s_noh * getNohAssociadoByIdAssociado(int id_associado)
{
	struct s_noh* ptemp;

	for (ptemp = listaA; ptemp->proximo != 0; ptemp = ptemp->proximo)
		if (id_associado == ((struct s_associado*)ptemp->dado)->id_associado)
		{
			return ptemp;
		}

	return 0;
};

struct s_noh* getNohLivroByIdLivro(id_livro)
{
	struct s_noh* ptemp;

	for (ptemp = listaL; ptemp->proximo != 0; ptemp = ptemp->proximo)
		if (id_livro == ((struct s_livro*)ptemp->dado)->id_livro)
		{
			return ptemp;
		}

	return 0;
};
