module Interruption(in, out, controlIO, controlPC, inst, CSe, clock, clock50, savePC, reset, CS);

	input in, out, controlIO, clock, clock50, reset;
	input [5:0] inst;
	output reg[1:0] controlPC;
	output reg CSe, savePC;

	output reg CS;
	reg[6:0] cont;
	
	initial begin
		CS = 1'b0;
		CSe = 1'b0;
		cont = 7'b0000000;
		savePC = 1'b0;
		controlPC = 2'b00;
	end
	
	always@ (in or out or controlIO)
	begin
		if((in == 1'b1 || out == 1'b1) && controlIO == 1'b1)
		begin
			controlPC = 2'b10;
		end
		else 
		begin
			controlPC = 2'b00;
		end
	end
	
//	always @ (posedge clock50)
//	begin
//		if(inst[31:26] == 6'b100100) CS = 1'b1;
//		else if(inst[31:26] == 6'b100101) CS = 1'b0;
//	end
	
	always @ (posedge clock)
	begin
	
		if(reset) 
		begin
			cont <= 7'd0;
			CS <= 1'b0;
			CSe <= 1'b0;
			savePC = 1'b0;
		end
		else if(inst == 6'b011001)
		begin
			cont <= 7'd0;
			CS <= 1'b0;
			CSe <= 1'b1;
			savePC = 1'b0;
		end
		else
		begin
			if(CS == 1'b0)
			begin
				if (inst == 6'b100100)
				begin
					CS <= 1'b1;
					cont = 7'd0;
					CSe = 1'b0;
					savePC = 1'b0;
				end
				else
				begin
					CS <= 1'b0;
					cont = 7'd0;
					CSe = 1'b0;
					savePC = 1'b0;
				end
			end
			else
			begin
				if (inst == 6'b100101)
				begin
					CS <= 1'b0;
				end
				else
				begin
					CS <= 1'b1;
					if (inst != 6'b010110 && inst != 6'b010111)
					begin
						if (cont == 7'd80)
						begin
							cont = 7'd0;
							CSe = 1'b1;
							savePC = 1'b0;
						end
						else if (cont == 7'd79)
						begin
							cont <= cont + 1;
							CSe = 1'b0;
							savePC = 1'b1;
						end
						else
						begin
							cont <= cont + 1;
							CSe = 1'b0;
							savePC = 1'b0;
						end
					end
					else
					begin
						CSe = 1'b0;
						savePC = 1'b0;
					end
				end
			end
		end
	end

endmodule