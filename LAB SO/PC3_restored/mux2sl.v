module mux2ls(a, b, escolhe, saida);
	
	input[4:0] a, b;
	input escolhe;
	output reg[4:0] saida;

	always @(*) begin
		case(escolhe) 
			1'b0: begin
				saida = a;
			end
			1'b1: begin
				saida = b;
			end 
		endcase
	end
endmodule