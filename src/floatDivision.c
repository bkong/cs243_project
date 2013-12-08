//tests performance when doing float division

int main(){
	float val1 = .345;
	float val2 = .894;
	
	int i;
	for(i=0; i<1000000000; i++){
		val1/val2;
	}

}
