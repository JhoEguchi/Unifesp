module muxPC(a, b, c, escolhe, saida);
	
	input[1:0] a, b, c;
	input[1:0] escolhe;
	output reg[1:0] saida;

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
			default saida = saida;
		endcase
	end
endmodule