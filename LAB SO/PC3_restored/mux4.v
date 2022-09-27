module mux4(a, b, c, d, escolhe, saida);
	
	input[31:0] a, b, c, d;
	input[1:0] escolhe;
	output reg[31:0] saida;

	always @(*) begin
		case(escolhe) 
			2'b00: begin
				saida = a;
			end
			2'b01: begin
				saida = b;
			end 
			2'b10: begin
				saida = c;
			end
			2'b11: begin
				saida = d;
			end
		endcase
	end
endmodule