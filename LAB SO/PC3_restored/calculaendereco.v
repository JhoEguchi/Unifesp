module calculaendereco(s1, s2, saida);
	
	input[11:0] s1, s2;
	output reg[11:0] saida;

	reg[11:0] res1;
	
	always @(*) begin
		res1 <= s1*128;
		saida <= res1 + s2;
	end
endmodule