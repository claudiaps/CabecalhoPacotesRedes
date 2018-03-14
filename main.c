#include <stdio.h>

//assinatura das funções
void read_print_Ethernet(FILE *file, unsigned char data[]);
void reade_print_IP(unsigned char dados[]);
void read_print_TCP(unsigned char tcp[]);


//inicio do programa

//função para ler e printar os campos do pacote ethernet
void read_print_Ethernet(FILE *file, unsigned char data[]){
	unsigned char destination[6];
	unsigned char source[6];
	unsigned char type[2];

	//le os campos do pacote ethernet do arquivo, destination, source, type, data 
	fread(destination, sizeof(unsigned char) * 6, 1, file);  
	fread(source, sizeof(unsigned char) * 6, 1, file);
	fread(type, sizeof(unsigned char) * 2, 1, file);	
	fread(data, sizeof(unsigned char) * 60, 1, file);

	//imprime os campos lidos anteriormente e armazenados nos arrays
	printf("Destination: ");
	for (int i = 0; i < 6; i++)
		printf("%x ", destination[i]);

	printf("\nSource: ");
	for (int i = 0; i < 6; i++)
		printf("%x ", source[i]);

	printf("\nType: ");
	for (int i = 0; i < 2; i++)
		printf("%x ", type[i]);

	printf("\nData: ");
	for (int i = 0; i < 60; i++)
		printf("%x", data[i]);

}

//função para ler e printar os campos do protocolo IP (o qual está na parte de dados do pacote ethernet)
void reade_print_IP(unsigned char dados[], unsigned char tcp[]){

	unsigned char cabecalho;

	cabecalho = dados[0] >> 4;
	printf("\nVersion: %x", cabecalho);

	cabecalho = dados[0] & 0x0f;
	printf("\nIHL: %x", cabecalho);
	int options = cabecalho * 32 / 8;

	cabecalho = dados[1] >> 2;
	printf("\nDSCP: %x", cabecalho);

	cabecalho = dados[1] & 0b00000011;
	printf("\nECN: %x", cabecalho);
	
	printf("\nTotal Lenght: %x%x", dados[2], dados[3]);

	printf("\nIdentification: %x%x", dados[4], dados[5]);

	cabecalho = dados[6] >> 5;
	printf("\nFlags: %x", cabecalho);
	
	cabecalho = dados[6] & 0b00011111;
	printf("\nOffset: %x%x", cabecalho, dados[7]);

	printf("\nTime to Live: %x", dados[8]);

	printf("\nProtocol: %x", dados[9]);
	
	printf("\nHeader Checksum: %x%x", dados[10], dados[11]);

	printf("\nSource: %x%x%x%x", dados[12], dados[13], dados[14], dados[15]);
		
	printf("\nDestination: %x%x%x%x", dados[16], dados[17], dados[18], dados[19]);

	int data_ip;
	int aux = options - 20;
	if (aux != 0){
		printf("\nOptions: %x%x%x%x", dados[20], dados[21], dados[22], dados[23]);
		data_ip = 24;
	}
	else{
		printf("\nOptions: null");
		data_ip = 20;
	}

	int y=0;
	printf("\nData: ");
	for(int i = data_ip; i<=60; i++){
		printf("%x", dados[i]);
		tcp[y] = dados[i];
		y++;
	}
}

void read_print_TCP(unsigned char tcp[]){

	unsigned char cabecalho;

	printf("\nSource: %x%x", tcp[0], tcp[1]);

	printf("\nDestiny: %x%x", tcp[2], tcp[3]);

	printf("\nSequence: %x%x%x%x", tcp[4], tcp[5], tcp[6], tcp[7]);

	printf("\nACK: %x%x%x%x", tcp[8], tcp[9], tcp[10], tcp[11]);
	
	cabecalho = tcp[12] >> 4;
	printf("\nHeader Lenght: %x", cabecalho);
	
	cabecalho = tcp[12] & 0b00001111;
	printf("\nFlags: %x%x", cabecalho, tcp[13]);

	printf("\nWindow Size: %x%x", tcp[14], tcp[15]);
	
	printf("\nChecksum: %x%x", tcp[16], tcp[17]);

	printf("\nUrgent Pointer: %x%x", tcp[18], tcp[19]);
	
	printf("\nOptions: %x%x%x%x", tcp[20], tcp[21], tcp[22], tcp[23]);

}

void read_print_UDP(unsigned char udp[]){
	
	printf("\nSource: %x%x", udp[0], udp[1]);

	printf("\nDestiny: %x%x", udp[2], udp[3]);

	printf("\nLenght: %x%x", udp[4], udp[5]);

	printf("\nChecksum: %x%x", udp[6], udp[7]);
	
	printf("\nData: ");
	for(int i=8; i<40; i++){
		printf("%x", udp[i]);
	}
	
}

int main()
{

	
	int op;
	printf("1: UDP\n2: TCP\n");
	scanf("%d", &op);

	FILE *file;
	unsigned char ethernet_data[60];
	unsigned char tcp_data[40];
	unsigned char udp_data[40];
	

	if (op == 1){
		file = fopen("packet_udp.bin", "rb");
		if (file == NULL)
		{
			printf("Problemas na abertura do arquivo\n");
			return -1;
		}

		printf("\n\n------Cabeçalho pacote Ethernet------\n\n");
		read_print_Ethernet(file, ethernet_data);

		printf("\n\n------Cabeçalho Protocolo IP------\n");
		reade_print_IP(ethernet_data, udp_data);

		printf("\n\n------Cabeçalho Protocolo UDP------\n");
		read_print_UDP(udp_data);		

	}

	else if (op == 2){
		file = fopen("packet_tcp.bin", "rb");
		if (file == NULL)
		{
			printf("Problemas na abertura do arquivo\n");
			return -1;
		}

		printf("\n\n------Cabeçalho pacote Ethernet------\n\n");
		read_print_Ethernet(file, ethernet_data);

		printf("\n\n------Cabeçalho Protocolo IP------\n");
		reade_print_IP(ethernet_data, tcp_data);

		printf("\n\n------Cabeçalho Protocolo TCP------\n");
		read_print_TCP(tcp_data);

	}

	return 0;

}