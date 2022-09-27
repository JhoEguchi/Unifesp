module bancoreg(escrita, rjal, rd, r1, r2, clock, write, PC, s1, s2, jump, wrenHD, writeHD, outHD, savePC);//resetCPU, a,b,c);

	input [4:0] rd, r1, r2;
	input clock, escrita, rjal, wrenHD, savePC; //resetCPU;
	input [31:0] write, writeHD;
	input [9:0] PC;
	output [31:0] s1, s2, jump, outHD;
	//output[6:0] a, b, c;
	
	reg [31:0] registradores[31:0];
	
	always @(negedge clock)
	begin
		if(escrita)
		begin
			registradores[rd] = write;
		end
		if(rjal)
		begin
			registradores[31] = PC+1;
		end
		if(wrenHD)
		begin
			registradores[registradores[17][4:0]] = writeHD;
		end
		if(savePC)
		begin
			registradores[23] = PC;
		end
	end
	
	//saida saida(.out(1), .numero(registradores[23]), .res(resetCPU), .saida1(a), .saida2(b), .saida3(c), .clock(clock));
	
	assign s1 = registradores[r1];
	assign s2 = registradores[r2];
	assign jump = registradores[rd];
	assign outHD = registradores[registradores[17][4:0]];
	
endmodule
	