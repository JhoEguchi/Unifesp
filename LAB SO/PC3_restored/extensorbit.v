module extensorbit(entrada1, entrada2, entrada3, selecao, saida);

	input[15:0] entrada1;
	input[20:0] entrada2;
	input[15:0] entrada3;
	input[1:0] selecao;
	output reg[31:0] saida;
	
	always @(*) begin
		case(selecao) 
			2'b00: begin
				saida = entrada1;
				if(entrada1[15] == 1'b1) saida = saida + 32'b11111111111111110000000000000000;
			end
			2'b01: begin
				saida = entrada2;
				if(entrada2[20] == 1'b1) saida = saida + 32'b11111111111000000000000000000000;
			end 
			2'b10: begin
				saida = entrada3;
				if(entrada3[15] == 1'b1) saida = saida + 32'b11111111111111110000000000000000;
			end
			default: saida = saida;
		endcase
	end
	
	endmodule
			