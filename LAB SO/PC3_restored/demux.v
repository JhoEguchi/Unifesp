module demux(entrada, escolhe, a, b, c);
	
	input[31:0] entrada;
	input escolhe;
	output reg[31:0] a, b, c;

	always @(*) begin
		case(escolhe) 
			2'b00: begin
				a = entrada;
			end
			2'b01: begin
				b = entrada;
			end 
			2'b10: begin
				c = entrada;
			end 
		endcase
	end
endmodule