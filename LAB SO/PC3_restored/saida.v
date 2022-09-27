module saida(out, numero, res, saida1, saida2, saida3, clock);

	input [31:0] numero;
	input res, out, clock;
	output reg[6:0] saida1, saida2, saida3;
	wire[3:0] unidade1, dezena1, centena1;
	wire[6:0] saidad1, saidad2, saidad3;
	
	initial
	begin
		saida1 = 7'b1111111;
		saida2 = 7'b1111111;
		saida3 = 7'b1111111;
	end
	
	conversorbin TX(.binario(numero), .dezena(dezena1), .unidade(unidade1), .centena(centena1));
	
	decodificador TY(.W(unidade1[3]),.X(unidade1[2]),.Y(unidade1[1]),.Z(unidade1[0]),.a(saidad1[0]),.b(saidad1[1]),.c(saidad1[2]),.d(saidad1[3]),.e(saidad1[4]),.f(saidad1[5]),.g(saidad1[6]));
	
	decodificador TZ(.W(dezena1[3]),.X(dezena1[2]),.Y(dezena1[1]),.Z(dezena1[0]),.a(saidad2[0]),.b(saidad2[1]),.c(saidad2[2]),.d(saidad2[3]),.e(saidad2[4]),.f(saidad2[5]),.g(saidad2[6]));
	
	decodificador TA(.W(centena1[3]),.X(centena1[2]),.Y(centena1[1]),.Z(centena1[0]),.a(saidad3[0]),.b(saidad3[1]),.c(saidad3[2]),.d(saidad3[3]),.e(saidad3[4]),.f(saidad3[5]),.g(saidad3[6]));
	
	always @(negedge clock) begin
	if(out == 1'b1)
	begin
		saida1 = saidad1;
		saida2 = saidad2;
		saida3 = saidad3;
	end
	if(res == 1'b1)
	begin
		saida1 = 7'b1111111;
		saida2 = 7'b1111111;
		saida3 = 7'b1111111;
	end
	end
	
endmodule
	
	