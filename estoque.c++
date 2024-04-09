#include <stdio.h>
#include <stdlib.h>

const int MAX=200;
int ordenado=0;// vari�vel p/ controlar a ordena��o dos dados
int tamanho=0, ponteiro_vendas=1;
int g=30; //variavel uzada para pagina��o

typedef struct TProduto{
    long int codigo;
    float quantidade;
    float pr_compra, pr_fornecedor;
    int tipo, quant_min, quantidade_vendido=0;
}Tproduto;

void leitura(Tproduto estoque[], int *tamanho);//gera o arquivo .dat na primeira vez
void gravacao(Tproduto estoque[], int tamanho);// realiza a grava��o dos dado no arquivo
int pesquisabinaria(Tproduto estoque[], int chave, int tamanho);
int vazio (int tamanho);
void ordena(Tproduto estoque[], int tamanho);//ordena o cadastro dos produtos por c�digo
void inclusao(Tproduto estoque[],int *tamanho);
void relatorio_geral (Tproduto estoque[], int tamanho);
void excluir(Tproduto estoque[], int *tamanho);
long int bucarcodigo2(Tproduto estoque[], int tamanho, long int exc);
void verifica_min(Tproduto estoque[], int tamanho);
void relatorio_expecial(Tproduto estoque[], int tamanho);
void venda(long int cod_vendido[], int quant_vendido[], float preco_vendido[], Tproduto estoque[], int tamanho);
void alterar(Tproduto estoque[], int tamanho);
const char* obterTipoProduto(int tipo);
void relatorio_vendas(long int cod_vendido[], int quant_vendido[], float preco_vendido[],  int qnt_vendas, Tproduto estoque[], int tamanho);
void paginacao(Tproduto estoque[], int i, int tamanho, void funcao(Tproduto estoque[], int tamanho));

int main(){
	Tproduto estoque[MAX];
	int qnt_vendas=0;
    int opcao;
    long int cod_vendido[ponteiro_vendas];
    int quant_vendido[ponteiro_vendas];
    float preco_vendido[ponteiro_vendas];
    
    leitura(estoque, &tamanho);// abre o arquivo da base de dados
    do{
        printf("\n 1- CADASTRAR\n");
    	printf(" 2- RELATORIO\n");
    	printf(" 3- EXCLUIR PRODUTO\n");
    	printf(" 4- VERIFICACAO DE ESTOQUE MINIMO\n");
    	printf(" 5- VERIFICACAO DE PRODUTOS COM PREJUISO\n");
    	printf(" 6- REALIZAR VENDA\n");
    	printf(" 7- ALTERAR PRODUTO\n");
    	printf(" 8- RELATORIO DE VENDAS\n");
        printf (" 0- SAIR\n");
        printf ("\nESCOLHA A OPCAO DESEJADA: ");
        scanf ("%d",&opcao);
        system("cls");
        switch (opcao){
            case 1:{
            	inclusao(estoque, &tamanho);
            	gravacao(estoque, tamanho);
				break;
			}        	
            case 2:{
            	
				relatorio_geral (estoque, tamanho);
				break;
			}	
            case 0: 
				printf("Obrigado por usar nosso sistema.\n");
				system("pause");
				exit(0);
				break;
		    case 3:
					excluir (estoque, &tamanho);
					gravacao(estoque, tamanho);
					break;
		    case 4:
		    	    verifica_min(estoque, tamanho);
		    	break;
		    case 5:
		    	    relatorio_expecial(estoque, tamanho);
		    	break;
		    case 6:
		    	    venda( cod_vendido, quant_vendido, preco_vendido, estoque, tamanho);
		    		gravacao(estoque, tamanho);
		    		qnt_vendas++;
		    		ponteiro_vendas++;
		    	break;
		    case 7:
		    	alterar(estoque, tamanho);
		    	gravacao(estoque, tamanho);
		    	break;
		   	case 8:
		   		relatorio_vendas(cod_vendido, quant_vendido, preco_vendido, qnt_vendas, estoque, tamanho);
                gravacao(estoque, tamanho);
		   		break;
            default: 
			    printf("OPCAO INVALIDA!\n");
            	system("pause");
            	system("cls");
        }
    }while (opcao!=0);
        gravacao(estoque, tamanho);
	return 0;
}

void leitura(Tproduto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   /* abrimos para anexar, bin�rio */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
//le os dados do arquivo
    while(!feof(arquivo)){
       fread (&estoque[*tamanho], sizeof (Tproduto) , 1, arquivo);
       (*tamanho)++;
    }
	(*tamanho)--;
//fecha o arquivo
	fclose(arquivo);
	return;
}

void gravacao(Tproduto estoque[], int tamanho){
    FILE *arquivo;
    int i;
    arquivo= fopen("estoque.dat", "w+b");	   /* abre e apaga o conte�do do arquivo,bin�rio */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
// grava a estrutura no arquivo
    for(i=0;i<tamanho;i++)  
       fwrite(&estoque[i], sizeof(Tproduto), 1, arquivo);
// Fecha o arquivo de dados
	fclose(arquivo);
	return;
}

int pesquisabinaria(Tproduto estoque[], int chave, int tamanho){
    if(vazio(tamanho))
       return -1;// vetor vazio       
    if (! ordenado){
        ordena(estoque,tamanho);// ordena
        ordenado=1;// variavel global
    }
    int inicio=0,final=tamanho, meio;
    while (inicio<=final){
        meio=(int)(inicio+final)/2;
        if (estoque[meio].codigo==chave)
           return meio; // encontrou
        if (estoque[meio].codigo<chave)
           inicio=meio+1;
        else
           final=meio-1;
    }
    return -1; // n�o encontrou
}

int vazio(int tamanho){
     if(tamanho==0){
        printf("\nREGISTRO VAZIO!\n");
        return 1;// vazio
     }
     return 0;// preenchido
}

void ordena(Tproduto estoque[], int tamanho){
	int i,j;
    Tproduto aux;
    for(i=0;i<tamanho-1;i++)
        for(j=i+1;j<tamanho;j++)
          	if (estoque[i].codigo>estoque[j].codigo){
              	aux=estoque[i];
              	estoque[i]=estoque[j];
              	estoque[j]=aux;
            }
}

void inclusao(Tproduto estoque[],int *tamanho){
             if (*tamanho==MAX){ 
                   printf ("\n ERRO! \n ARQUIVO CHEIO.\n");
                   return;
             } 
             Tproduto aux;
             char correto='n';
             do{
                  printf ("Codigo ..............................: ");
                  scanf ("%ld",&aux.codigo);
				  fflush(stdin);
            }while (aux.codigo <= 0);
            if (pesquisabinaria(estoque, aux.codigo, *tamanho) <0){
                  do{
                        printf ("Quantidade do produto ...............: ");
                        scanf ("%f",&aux.quantidade);
						fflush(stdin);
                    }while(aux.quantidade<0);
                    
                    do{
                        printf ("Quantidade minima de estoque ...............: ");
                        scanf ("%i",&aux.quant_min);
						fflush(stdin);
                    }while(aux.quant_min<0);
                    
                  do{
                  		printf ("Preco de venda do produto ..........: ");
                  		scanf ("%f",&aux.pr_compra);
						fflush(stdin);
				  }while(aux.pr_compra<0);
				  do{
                  		printf ("Preco de compra com fornecedor ..........: ");
                  		scanf ("%f",&aux.pr_fornecedor);
						fflush(stdin);
				  }while(aux.pr_fornecedor<0);
				  printf("-----------------------------\n");
				  printf("\n1:Anel \n2:Brinco \n3:Corrente \n4:Alianca \n5:Relogio \n6:Pulseira \n7:Colar \n8:Conjuntos \n9:Pircing \n10:Oculos \n");
				  printf("-----------------------------\n");
				  printf("tipo do produto:");
				  scanf("%i", & aux.tipo);
				   fflush(stdin);   
              while(aux.tipo>=11 && aux.tipo<=0){
              	printf("Numero n valido!!\n");
              	 printf("-----------------------------\n");
				  printf("1:Anel \n2:Brinco \n3:Corrente \n4:Alianca \n5:Relogio \n6:Pulseira \n7:Colar \n8:Conjuntos \n9:Pircing \n10:Oculos \n");
				  printf("-----------------------------\n");
				  printf("Tipo do produto:");
				  scanf("%i", & aux.tipo);
				   fflush(stdin);   
			  }
              
				  printf("Os dados estao corretos?(S/N):");
                  correto=getchar();
				  fflush(stdin);     
				  system("cls");
                  
                  if (correto=='s'||correto=='S'){
                       estoque [*tamanho] = aux;
                       (*tamanho) ++;
                       ordenado=0;
                       printf ("\tO PRODUTO FOI INCLUIDO!\n");
					fflush(stdin);
                  }
			}else 
    			printf ("\tCODIGO JA CADASTRADO!!\n");
    printf("\tAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("cls");
    return;
}

void relatorio_geral (Tproduto estoque[], int tamanho){
	
	if(vazio(tamanho)==1){
		return;
   }
    int i;
    printf("=======================================================================\n");
	printf("\t\tControle de Estoque - Relatorio geral\n");
    printf("=======================================================================\n");
    ordena(estoque, tamanho);
    
    for(i=0; i<tamanho; i++){
    	paginacao(estoque, i, tamanho, relatorio_geral);
    	printf("Codigo:%ld \nQuantidade:%.2f\n", estoque[i].codigo, estoque[i].quantidade);
    	printf("Preco do fornecedor:%.2f\t\t\t\tPreco de venda:%.2f\n", estoque[i].pr_fornecedor, estoque[i].pr_compra);
    	printf("Tipo: %s\n", obterTipoProduto(estoque[i].tipo));
    	printf("-----------------------------------------------------------------------\n\n");
	}
     printf("-----------------------------------------------------------------------\n\n");
    fflush(stdin);
    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("cls");
}

void excluir(Tproduto estoque[], int *tamanho){
	
	if(vazio(*tamanho)==1){
		return;
   }
    long int exc;
    char sim='n';

    printf("Codigo do produto que deseja excluir: ");
    scanf("%ld", &exc);

    ordena(estoque, *tamanho);

    int indice = bucarcodigo2(estoque, *tamanho, exc);
    if (indice < 0 || indice >= *tamanho) {
        printf("Codigo invalido!\n");
    } else {
        printf("\n\tCodigo: %li\n", estoque[indice].codigo);
        printf("\tQuantidade: %.2f\n", estoque[indice].quantidade);
        printf("\tPreco: %.2f\n", estoque[indice].pr_compra);
        printf("\tTipo: %s\n", obterTipoProduto(estoque[indice].tipo));

        printf("\nDeseja excluir esse produto? (S/N): ");
        scanf(" %c", &sim);
        fflush(stdin);

        if (sim == 's' || sim == 'S') {
            for (int i = indice; i < *tamanho - 1; i++) {
                estoque[i] = estoque[i + 1];
            }
            (*tamanho)--;
            printf("Produto excluido com sucesso!\n");
        } else {
            printf("Exclusao cancelada.\n");
        }
    }

    printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("cls");
    return;
}
     
void relatorio_expecial(Tproduto estoque[], int tamanho){
	
	if(vazio(tamanho)==1){
		return;
   }
	int x=0;
	system("cls");
	printf("------PRODUTOS SENDO VENDIDOS COM PREJUISO------\n\n");
	printf("-------------------------------------------------------\n");
    for(int i=0; i<tamanho; i++){
    	if(estoque[i].pr_compra<estoque[i].pr_fornecedor){
			printf("Codigo:%ld \nQuantidade:%.2f\t\t\t\t\tQuantidade Minima:%i\n", estoque[i].codigo, estoque[i].quantidade, estoque[i].quant_min);
    	printf("Preco do fornecedor:%.2f\t\t\t\tPreco de venda:%.2f\n", estoque[i].pr_fornecedor, estoque[i].pr_compra);
    	printf("Tipo: %s\n", obterTipoProduto(estoque[i].tipo));
    	printf("-----------------------------------------------------------------------\n\n");
    	x=x+1;
		}
	}
	if(x==0){
		printf("\nNENHUM PRODUTO ESTA SENDO VENDIDO COM PREJUISO\n");
	}
	printf("-----------------------------------------------------------------------\n\n");
    fflush(stdin);
    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("cls");
}

long int bucarcodigo2(Tproduto estoque[], int tamanho, long int exc){
	for(int i=0; i<tamanho; i++){
		if(estoque[i].codigo==exc){
			return i;
		}
	}
	return -1;
}
  
void verifica_min(Tproduto estoque[], int tamanho){
	
	if(vazio(tamanho)==1){
		return;
   }
	int x=0;
	system("cls");
	printf("------PRODUTOS COM QUANTIDADE MINIMA ATINGIDA------\n\n");
	printf("-------------------------------------------------------\n");
	for(int i=0; i<tamanho; i++){
		if(estoque[i].quantidade<estoque[i].quant_min){
			printf("Codigo:%ld \nQuantidade:%.2f\t\t\t\t\tQuantidade Minima:%i\n", estoque[i].codigo, estoque[i].quantidade, estoque[i].quant_min);
    	printf("Preco do fornecedor:%.2f\t\t\t\tPreco de venda:%.2f\n", estoque[i].pr_fornecedor, estoque[i].pr_compra);
    	printf("Tipo: %s\n", obterTipoProduto(estoque[i].tipo));
    	printf("-----------------------------------------------------------------------\n\n");
    	x=x+1;
        }
	}
	if(x==0){
		printf("\nNENHUM PRODUTO ESTA ABAIXO DA QUANTIDADE MINIMA\n");
	}
	printf("-----------------------------------------------------------------------\n\n");
    fflush(stdin);
    printf("APERTE ENTER PARA VOLTAR AO MENU");
    getchar();
    system("cls");
}
  
void venda(long int cod_vendido[], int quant_vendido[], float preco_vendido[], Tproduto estoque[], int tamanho){
	
	if(vazio(tamanho)==1){
		return;
   }
	char s='n';
	long int cod;
	int quant, i=0;
	float preco;
	printf("Codigo do Produto: ");
	scanf("%ld", &cod);
	int indice = bucarcodigo2(estoque, tamanho, cod);
	
	if (indice < 0 || indice >= tamanho) {
        printf("\nCodigo invalido!\n");
    } else {
    	
	if((estoque[indice].quantidade)>0){
	
    	printf("\n-----------------------------------");
        printf("\n\tCodigo: %li\n", estoque[indice].codigo);
        printf("\tQuantidade: %.2f\n", estoque[indice].quantidade);
        printf("\tPreco: %.2f\n", estoque[indice].pr_compra);
        printf("\tTipo: %s\n", obterTipoProduto(estoque[indice].tipo));
        printf("-----------------------------------\n\n");
    
    printf("Quantidade a ser Vendida: ");
    scanf("%i", &quant);
    while(quant>estoque[indice].quantidade || quant<=0){
    	printf("\tA Quantidade Vendida tem que ser Maior que Zero e Menor que a Quantidade Cadastrada!\n");
    	printf("Quantidade a ser Vendida: ");
    scanf("%i", &quant);
	}
	
    printf("Preco de Venda: ");
    scanf("%f", &preco);
    while(preco<0){
    	printf("O Preco nao pode ser Negativo!\n");
    	printf("Preco de Venda: ");
    scanf("%f", &preco);
	}
    
    printf("\n\nDeseja Realizar a Venda? (S/N): ");
        scanf(" %c", &s);
        fflush(stdin);

        if (s == 's' || s == 'S'){
    (estoque[indice].pr_compra)=preco;
    (estoque[indice].quantidade)=(estoque[indice].quantidade)-quant;
    printf("Venda Realizada!\n\n");
    
    cod_vendido[i]=cod;
    quant_vendido[i]=quant;
    preco_vendido[i]=preco;
    estoque[indice].quantidade_vendido++;
    i++;
    } else {
    	printf("Venda Cancelada!\n\n");
    	}
    } else {
	printf("Produto Esgotado!");
}
}
	
	printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("cls");
    return;
}

void alterar(Tproduto estoque[], int tamanho){
	int opc;
	long int cod;
	
	printf("Codigo do Produto que Deseja Alterar: ");
	scanf("%li", &cod);
	system("cls");
	int i = bucarcodigo2(estoque, tamanho, cod);
	
	if (i < 0 || i >= tamanho) {
        printf("\nCodigo invalido!\n");
        
    } else {
	
    	printf("Codigo:%ld \nQuantidade:%.2f\t\t\t\t\tQuantidade Minima:%i\n", estoque[i].codigo, estoque[i].quantidade, estoque[i].quant_min);
    	printf("Preco do fornecedor:%.2f\t\t\t\tPreco de venda:%.2f\n", estoque[i].pr_fornecedor, estoque[i].pr_compra);
    	printf("Tipo: %s\n", obterTipoProduto(estoque[i].tipo));
    	printf("-----------------------------------------------------------------\n\n");
    	printf("1 - Codigo\t 2 - Quantidade\t 3 - Quantidade Minima\t 4 - Preco de Venda\t 5 - Preco do Fornecedor\t \n6 - Tipo\t 7 - Tudo\t 8 - Voltar ao Menu\n\n");
    	printf("O que Deseja Alterar? ");
    	scanf("%i", &opc);
    	switch(opc){
    		case 1:
    			do{
                  printf ("Novo Codigo: ");
                  scanf ("%ld",&estoque[i].codigo);
				  fflush(stdin);
            }while (estoque[i].codigo <= 0);
    			break;
    		case 2:
    			do{
                  printf ("Nova Quantidade: ");
                  scanf ("%f",&estoque[i].quantidade);
			    	fflush(stdin);
                    }while(estoque[i].quantidade<0);
    			break;
        	case 3:
    			do{
                        printf ("Nova Quantidade Minima: ");
                        scanf ("%i",&estoque[i].quant_min);
						fflush(stdin);
                    }while(estoque[i].quant_min<0);
    			break;
    		case 4:
    			do{
                  		printf ("Novo Preco de Venda: ");
                  		scanf ("%f",&estoque[i].pr_compra);
						fflush(stdin);
				  }while(estoque[i].pr_compra<0);
    			break;
    		case 5:
    			do{
                  		printf ("Novo Preco do Fornecedor: ");
                  		scanf ("%f",&estoque[i].pr_fornecedor);
						fflush(stdin);
				  }while(estoque[i].pr_fornecedor<0);
    			break;
    		case 6:
    			printf("-----------------------------\n");
				  printf("\n1:Anel \n2:Brinco \n3:Corrente \n4:Alianca \n5:Relogio \n6:Pulseira \n7:Colar \n8:Conjuntos \n9:Pircing \n10:Oculos \n");
				  printf("-----------------------------\n");
    			printf("Novo Tipo: ");
    			scanf("%i", &estoque[i].tipo);
    			while(estoque[i].tipo>=11 && estoque[i].tipo<=0){
    				system("cls");
              	printf("Numero n valido!!\n");
              	 printf("-----------------------------\n");
				  printf("1:Anel \n2:Brinco \n3:Corrente \n4:Alianca \n5:Relogio \n6:Pulseira \n7:Colar \n8:Conjuntos \n9:Pircing \n10:Oculos \n");
				  printf("-----------------------------\n");
				  printf("Novo Tipo:");
				  scanf("%i", & estoque[i].tipo);
				   fflush(stdin);   
			  }
    			break;
    		case 7:
    			do{
                  printf ("Novo Codigo: ");
                  scanf ("%ld",&estoque[i].codigo);
				  fflush(stdin);
            }while (estoque[i].codigo <= 0);
    			do{
                  printf ("Nova Quantidade: ");
                  scanf ("%f",&estoque[i].quantidade);
			    	fflush(stdin);
                    }while(estoque[i].quantidade<0);
    			do{
                        printf ("Nova Quantidade Minima: ");
                        scanf ("%i",&estoque[i].quant_min);
						fflush(stdin);
                    }while(estoque[i].quant_min<0);
    			do{
                  		printf ("Novo Preco de Venda: ");
                  		scanf ("%f",&estoque[i].pr_compra);
						fflush(stdin);
				  }while(estoque[i].pr_compra<0);
    			do{
                  		printf ("Novo Preco do Fornecedor: ");
                  		scanf ("%f",&estoque[i].pr_fornecedor);
						fflush(stdin);
				  }while(estoque[i].pr_fornecedor<0);
    			
    			printf("-----------------------------\n");
				  printf("\n1:Anel \n2:Brinco \n3:Corrente \n4:Alianca \n5:Relogio \n6:Pulseira \n7:Colar \n8:Conjuntos \n9:Pircing \n10:Oculos \n");
				  printf("-----------------------------\n");
    			printf("Novo Tipo: ");
    			scanf("%i", &estoque[i].tipo);
    			while(estoque[i].tipo>=11 && estoque[i].tipo<=0){
    				system("cls");
              	printf("Numero n valido!!\n");
              	 printf("-----------------------------\n");
				  printf("1:Anel \n2:Brinco \n3:Corrente \n4:Alianca \n5:Relogio \n6:Pulseira \n7:Colar \n8:Conjuntos \n9:Pircing \n10:Oculos \n");
				  printf("-----------------------------\n");
				  printf("Novo Tipo: ");
				  scanf("%i", & estoque[i].tipo);
				   fflush(stdin);  
				   
				   
				    printf("Produto Alterado!!\n");
			  }
    			break;
    		case 8:
    			system("cls");
    			return;
    			break;
    		default:
    				printf("opc invalida\n");
    				return;
		}
    	
}
    printf("\nAPERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("cls");
    return;
}

const char* obterTipoProduto(int tipo){
    switch (tipo) {
        case 1:
            return "Anel";
        case 2:
            return "Brinco";
        case 3:
            return "Corrente";
        case 4:
            return "Alianca";
        case 5:
            return "Relogio";
        case 6:
            return "Pulseira";
        case 7:
            return "Colar";
        case 8:
            return "Conjunto";
        case 9:
            return "Piercing";
        case 10:
            return "Oculos";
        default:
            return "Desconhecido";
    }
}

void relatorio_vendas(long int cod_vendido[], int quant_vendido[], float preco_vendido[], int qnt_vendas, Tproduto estoque[], int tamanho) {
    if (qnt_vendas == 0) {
        printf("\nNenhuma Venda Realizada.\n");
        return;
    }

    printf("=======================================================================\n");
    printf("\t\tRelatorio de Vendas\n");
    printf("=======================================================================\n");

    for (int i = 0; i < qnt_vendas; i++) {
        int indice = pesquisabinaria(estoque, cod_vendido[i], tamanho);
        if (indice >= 0) {
            printf("Codigo: %ld\n", estoque[indice].codigo);
            printf("Quantidade Vendida: %d\t\tQuantidade Restante: %,2f \n", quant_vendido[i], estoque[indice].quantidade);
            printf("Preco Unitario: %.2f\t\t Preco Atual do Produto: \n", preco_vendido[i], estoque[indice].pr_compra);
            printf("Total: %.2f\n", quant_vendido[i] * preco_vendido[i]);
            printf("Tipo: %s\n", obterTipoProduto(estoque[indice].tipo));
            printf("---------------------------------------------\n");
        }
    }

    printf("-----------------------------------------------------------------------\n\n");
    printf("APERTE ENTER PARA VOLTAR AO MENU");
    fflush(stdin);
    getchar();
    system("cls");
}

void paginacao(Tproduto estoque[], int i, int tamanho, void funcao(Tproduto estoque[], int tamanho)){
	int j=30, opc=0;
	if (i==15){
		printf("\n     1 - Proxima Pagina       ");
		scanf("%i", &opc);
		if(opc==1){
			system("cls");
			g=30;
		}
    } else if (i==g) {
		printf("\n     1 - ir para a Primeira Pagina\t\t 2 - Proxima Pagina       ");
		scanf("%i", &opc);
		g=30;
		if(opc==1){
			system("cls");
			g=30;
			funcao (estoque, tamanho);
		} else if(opc==2){
			system("cls");
			g=g+15;
			return;
		}
		return;
		}	
	}
}