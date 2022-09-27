module PC(endereco, clock, flag, novoendereco, reset, resetBIOS, CSe);
	
	input clock, reset, resetBIOS, CSe;
	input[31:0] novoendereco;
	input[1:0] flag;
	output reg[9:0] endereco;
	
	initial
	begin
	endereco = 10'b0000000000;
	end
	
	always @(negedge clock) 
	begin
		if(CSe == 1'b0)
		begin
			if(reset)
			begin
				endereco <= 10'b0000000000;
			end
			else if(resetBIOS)
			begin
				endereco <= 10'b0000000000;
			end
			else if(flag == 2'b00)
			begin
				endereco <= endereco + 1;
			end
			else if(flag == 2'b01)
			begin
				endereco = novoendereco[9:0];
			end
			else if(flag == 2'b10)
			begin
				endereco <= endereco;
			end
		end
		else
		begin
			if(reset)
			begin
				endereco <= 10'b0000000000;
			end
			else if(resetBIOS)
			begin
				endereco <= 10'b0000000000;
			end
			else endereco <= 10'b0001011010;
		end
	end
endmodule
	
	

	