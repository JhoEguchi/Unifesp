module debounce(button, clock, saida);

	input button, clock;
	output saida;
	reg q1, q2, q3;
	
	always @(posedge clock) begin
		q1 <= button;
		q2 <= q1;
		q3 <= q2;
	
	end
		
	assign saida = q1 & q2 & q3;
	
endmodule
	