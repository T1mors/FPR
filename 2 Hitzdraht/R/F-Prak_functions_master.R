## *************************************************************************************
##  Einige kleine Funktionen, um die gemessenen Daten der Messungen zu verarbeiten
## *************************************************************************************
## *************************************************************************************
load_data <- function(file ="")
{
	load(file)
	name <- load(file)	
	cmd <- parse(text=paste("inputdata <-", name, sep=""))
	eval(cmd)
	return(inputdata)
}
## *************************************************************************************
##	Kalibrieren der Hitzdraht Daten

##	============================================
##	Die Kalibrierung mit der Funktion
##	const + m*(x - x0)^4

##	file: 		Pfad der zu kalibrierenden Daten
##	const:		const der Kalibrierfunktion
##	m:			m der Kalibrierfunktion
##	x0:			x0 der Kalibriefunktion
##	============================================
HW_kali <- function(file="", const = 0, m = 1, x0 = 0)
{
	data <- load_data(file)
	data <- const + m*(data - x0)^(4)
	
	print(paste("Mittelwert: ",mean(data)," m/s"))
	print(paste("Standardabweichung: ",sd(data), " m/s"))
	
	file <- strsplit(file,split=".Rdata")
	save(data, file = paste(file,"HW_kali.Rdata",sep="_"))
}

## *************************************************************************************
#######################################################################################################
# Plotten eines Zeitausschnitts einer Zeitreihe
# die Parameter start und ende geben dabei die Zeiten in Sekunden wieder
plotN<-function(file="", start = 0, ende = 1, Samplefrequenz=10000)
{
	x <- load_data(file)
	start_sec = start
	ende_sec = ende
	time <- seq(1/Samplefrequenz,(length(x)/Samplefrequenz),1/Samplefrequenz)

	start <- (start*Samplefrequenz) + 1
	ende <- (ende*Samplefrequenz)
	
	plot(time[start:ende],x[start:ende], type ="l")
	
	file <- strsplit(file,split="_HW_kali.Rdata")
	write.table(list(time[start:ende], x[start:ende]), paste(file,"time_plot_start=",start_sec,"_ende=",ende_sec,".txt",sep="_"), col.names =c("time[s]","vel[m/s]"), row.names = FALSE)
}

#######################################################################################################
# Spektrum über Fenster von fftN Punkten gemittelt
fftN<-function(file="", fftN=8192, Samplefrequenz=10000)
{
	x <- load_data(file)

	SN<-floor(length(x)/(fftN*2));
	Spectrum<-seq(0,length=fftN,by=0);
	for(i in 1:SN)
	{
		dummy<-spectrum(x[((i-1)*2*fftN+1):(i*2*fftN)],plot=FALSE,fast=TRUE);
		Spectrum<-Spectrum+dummy$spec;
		if((i %% 10) == 0) plot(dummy$freq* Samplefrequenz ,Spectrum,log="xy", type="l");
	}
	plot(dummy$freq* Samplefrequenz ,Spectrum,log="xy", type="l", xlab="f[Hz]", ylab="P(f)");
	f<-dummy$freq[(fftN/2):fftN] * Samplefrequenz;
	Pf<-Spectrum[(fftN/2):fftN];
    file <- strsplit(file,split="_HW_kali.Rdata")
	write.table(list(f=dummy$freq* Samplefrequenz,P=Spectrum), paste(file,"HW_fft.txt",sep="_"), col.names =c("Freq","P(f)"), row.names = FALSE)
	return(list(f=dummy$freq* Samplefrequenz,P=Spectrum))
}

