#include <WProgram.h>

static bool program_done = false;

int main(void)
{
	init();

	setup();
    
	while(!program_done)
		loop();
        
	return 0;
}

void done(void)
{
program_done = true;
}
