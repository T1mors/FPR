## *************************************************************************************
##  Funktion zum trennen der Kanäle und Abspeichern als .RData Format
## *************************************************************************************
data_split <- function(file ="", channels = 2)
{
  data <- as.matrix(read.table(file))
  file <- strsplit(file,".txt")

  if (channels > 1)
  {
    for (i in 1:channels)
    {
      cmd <- parse(text=paste("Ch",i,"<- data[,",i,"]",sep =""))
      eval(cmd)
 
      newfile <- paste(file, "_Ch",i,".Rdata",sep="")
      cmd <- parse(text=paste("save(Ch",i,",file = newfile)",sep=""))
      eval(cmd)	
    }
  }
  else 
  {
    Ch1 <- data

    newfile <- paste(file, "_Ch1.Rdata",sep="")
    save(Ch1,file = newfile)

  }
}

## *************************************************************************************
convert_and_split <- function(folder, group.name = NA, channels = 2)
{
    setwd(folder)
    files = dir(pattern = ".txt")
    for(n1 in files)
    {
      data_split(file = n1, channels = channels)
    }
    if(!is.na(group.name))
    {
      dir.create(paste(group.name, "/Rdata", sep = ""))
      Rdata.files = dir(pattern = ".Rdata")
      for(n1 in Rdata.files)
      {
        file.rename(n1, paste(group.name, "/Rdata/", n1, sep = ""))
      }
    }
  
}
## *************************************************************************************