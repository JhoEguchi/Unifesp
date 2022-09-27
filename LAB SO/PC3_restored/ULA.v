module ULA(dado1, dado2, operacao, saida, igual);

	input [31:0] dado1, dado2;
	input [3:0] operacao; //vem da unidade de controle
	output reg[31:0] saida;
	output reg igual;
	
	initial
		igual = 1'b0;
	
	always @(operacao or dado1 or dado2)
	begin
		case(operacao)
			4'b0000 : 
			begin
				saida = dado1 + dado2; 
				igual = 1'b0;
			end
			4'b0001 :
			begin
				saida = dado1 - dado2; 
				igual = 1'b0;
			end
			4'b0010 : 
			begin
			saida = dado1 & dado2;	
			igual = 1'b0;
			end
			4'b0011 : 
			begin
			saida = dado1 | dado2;	
			igual = 1'b0;
			end
			4'b0100 : //sl
			begin
			saida = dado1 << 1; 
			igual = 1'b0; 
			end
			4'b0101 : //sr
			begin
			saida = dado1 >> 1;	
			igual = 1'b0; 
			end
			4'b0110 :
			begin	
				if(dado1 == dado2) igual = 1'b1;	//beq
				else igual = 1'b0;
				saida = 0;
			end
			4'b0111 :
			begin
				if(dado1 != dado2) igual = 1'b1;	//bne
				else igual = 1'b0;
				saida = 0;
			end
			4'b1000 : begin
			if(dado1 < dado2)
			begin
				saida = 32'b00000000000000000000000000000001; 
				igual = 1'b0;
			end
			else
			begin 
				saida = 32'b00000000000000000000000000000000;
				igual = 1'b0;
			end
			end
			4'b1001 :
			begin
			saida = ~dado1; 
			igual = 1'b0;
			end
			default :
			begin
				saida = 0;
				igual = 1'b0;
			end
		endcase
		end
endmodule
						
	