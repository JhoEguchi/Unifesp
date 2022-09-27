module adder(a, b, saida);
	
	input[31:0] a,b;
	output reg[31:0] saida;

	always @(*) begin
		saida = a + b;
	end
endmodule