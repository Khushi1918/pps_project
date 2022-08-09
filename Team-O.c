/* All rights reserved.
 * Team O(1)
 * KHUSHI KUMARI - BTECH/10369/21
 * RISHAV SARKAR - BTECH/10404/21
 * SHRUTI SHIKHA - BTECH/10357/21
 * ANIKET SOURAV - BTECH/10512/21
 * Additional Info ----> Space Complexity: O(N^2); Time Complexity: O(N^2) (Expected)
 * VERSION 2.1.0 (Complete) 
 * Total Hours Spent here - 20
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure for PGMB image file
typedef struct imgPGMB 
{
    FILE* file;
	char imgType[3];
	unsigned int* data;
	unsigned int pixel_max;
	unsigned int w;
	unsigned int H;	
    long pos;
	long end_pos;
} imgPGMB;

//Function to skip the comments
void skipComm(FILE* fp)
{
	int ch;
	char line[100];

	// Ignore any blank lines
	while ((ch = fgetc(fp)) != EOF && isspace(ch));

	//Recursively ignore the comments starting with '#'
	if (ch == '#') 
	{
		fgets(line, sizeof(line), fp);
		skipComm(fp);
	}
	else
		fseek(fp, -1, SEEK_CUR);
}

//Function to read the header
void readHeader(imgPGMB* img)
{
	skipComm(img->file);
	fscanf(img->file, "%s", img->imgType);

	//Check for the file type
	if (strcmp(img->imgType, "P5")) 
    {
		printf("Wrong file type!\n");
		exit(1);
	}

	skipComm(img->file);

	// Read the image dimensions
	fscanf(img->file, "%d %d", &(img->w), &(img->H));

	skipComm(img->file);

	// Read maximum gray value
	fscanf(img->file, "%d", &(img->pixel_max));
	skipComm(img->file);

    img->pos = ftell(img->file); //Taking the position of the cursor for reference
	fseek(img->file, 0, SEEK_END);
	img->end_pos = ftell(img->file);
	fseek(img->file, 0, img->pos);

}

//Reading the binary file
void readBin(imgPGMB* img)
{
	int i = 0;
	unsigned char ch;

    fseek(img->file, 0, img->pos);
	while((ch=getc(img->file))!=EOF&&(i < img->w*img->H))
    {
        int n = (int) ch;
        *(img->data+i) = n;
		i++;
    }
}

//Function to scale up the matrix
void matrixScaling(imgPGMB* img, int scalingFact)
{
	int i = 0, j = 0;
    for(i = 0; i < img->H; i++)
    {
        for(j = 0; j < img->w; j++)
        {
            *(img->data+i+j) *= scalingFact;

            if((*img->data+i+j) >= 255)
                *(img->data+i+j) = 255;
        }
    }
}

//Function to write to the binary file
void writeBin(imgPGMB* img)
{
	int i = 0, r = 0, c = 0;
	unsigned char ch;

    fseek(img->file, 0, img->pos); //Repositioning the cursor to the begining of the binary values set
    while(ftell(img->file) != img->end_pos)
    {
		ch = (unsigned char) *(img->data+r+c);
		fwrite(&ch, sizeof(unsigned char), 1, img->file);

		if(c == img->w-1)
		{
			r++;
			c = 0;
		}
        
    }
}

// Function to print the file details
void printHeader(imgPGMB* img)
{
	printf("PGM File type : %s\n",img->imgType);

	if (!strcmp(img->imgType, "P2"))
		printf("PGM File Format: ASCII\n");

	else if (!strcmp(img->imgType, "P5"))
		printf("PGM File Format: Binary\n");

	printf("Image Dimensions: %d x %d\n", img->w, img->H);
	printf("Max Pixel Value : %d\n",img->pixel_max);
}

// Driver Code
int main()
{
    int sc_fact = 0; //Scaling factor
    char addr_buffer[256]; //In accordance with the MAX_PATH_LENGTH limit for Windows
    imgPGMB img; //Declaring struct img for storing the header info
    
    //Taking user input for file path and scaling factor
    printf("Enter the file path: ");
    scanf("%[^\n]%*c", addr_buffer);
    printf("Enter the scaling factor: ");
    scanf("%d", &sc_fact);

	img.file = fopen(addr_buffer, "rb+"); //Opening the file stream in r+b mode

	if(img.file == NULL)
	{
		printf("The file does not exist!!");
		exit(1);
	}
	else
	{
		printf("\n");
		printf("Reading the header file....\n");
		readHeader(&img); //Read the header file

		printf("Printing the header file....\n");
		printHeader(&img); //Print the header file
		printf("\n");

		img.data = (int*)malloc(sizeof(int)*img.H*img.w);

		printf("Reading the binary values...\n");
		readBin(&img); //Reading the binary values

		printf("Scaling up the matrix values...\n");
		matrixScaling(&img, sc_fact); //Scaling the matrix by the scaling factor

		fseek(img.file, 0, img.end_pos);
		printf("Writing the binary values....\n");
		writeBin(&img); //Writing it to the file
		
		printf("Image successfully scaled.");
		fflush(img.file);
	}

	fclose(img.file);
    printf("\n");
	return 0;
}