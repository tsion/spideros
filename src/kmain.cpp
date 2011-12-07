extern "C" void kmain(void* /*mbd*/, u32 magic)
{
   if ( magic != 0x2BADB002 )
   {
      /* Something went not according to specs. Print an error */
      /* message and halt, but do *not* rely on the multiboot */
      /* data structure. */
   }

   /* You could either use multiboot.h */
   /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
   /* or do your offsets yourself. The following is merely an example. */ 
   //char * boot_loader_name =(char*) ((long*)mbd)[16];

   /* Print a letter to screen to see everything is working: */
   unsigned char *videoram = (unsigned char *) 0xb8000;
   int arr[5] = {0, 2, 4, 6, 8};
   for(int x : arr) {
      videoram[x] = 65; /* character 'A' */
      videoram[x+1] = 0x07; /* light grey (7) on black (0). */
   }
}

