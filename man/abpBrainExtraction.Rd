% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/abpBrainExtraction.R
\name{abpBrainExtraction}
\alias{abpBrainExtraction}
\title{An ants-based brain extraction script.}
\usage{
abpBrainExtraction(img = NA, tem = NA, temmask = NA, temregmask = NA,
  regtype = "SyN", tdir = NA, num_threads = 1)
}
\arguments{
\item{img}{whole head image to which we map a brain mask}

\item{tem}{Template image (the whole head) which has an associated label mask.}

\item{temmask}{Template's antsImage brain mask.}

\item{temregmask}{Template's registration mask including skull but not the face}

\item{regtype}{registration type: 'SyN' (fast, default), 'SyNabp' (better, slower)}

\item{tdir}{temporary directory (optional)}

\item{num_threads}{will execute 
\code{Sys.setenv(ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS = num_threads)} before
running to attempt a more reproducible result.  See
\url{https://github.com/ANTsX/ANTs/wiki/antsRegistration-reproducibility-issues}
for discussion.  If \code{NULL}, will not set anything.}
}
\value{
outputs a brain image and brain mask.
}
\description{
Brain extraction based on mapping a template image and its mask to the input
image.  Should be preceded by abpN4.
}
\examples{

Sys.setenv(ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS = 1)
set.seed(1)
n = 64
fn<-getANTsRData("r16")
img<-antsImageRead(fn)
img<-resampleImage(img,c(n,n),1,0)
tf<-getANTsRData("r27")
tem<-antsImageRead(tf)
tem<-resampleImage(tem,c(n,n),1,0)
temmask<-antsImageClone( tem )
temmask[ tem  > 20 ] <- 1
temmask[ tem  <= 20 ] <- 0
bm<-abpBrainExtraction(img=img,tem=tem,temmask=temmask, num_threads = 1)
bm2<-abpBrainExtraction(img=img,tem=tem,temmask=temmask, num_threads = 1)

}
\author{
Tustison N, Avants BB
}
