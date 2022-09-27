module select5bits(a, saida);
	
	input[31:0] a;
	output reg[4:0] saida;

	always @(*) begin
		saida = a[4:0];
	end
endmodule