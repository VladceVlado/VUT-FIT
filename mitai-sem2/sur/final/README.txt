autori: Vladimír Dušek, David Ling, Richard Mička,Tomáš Strych

zdrojove subory - ./SRC
vysledne textove dokumenty - ./RESULTS
augmentovane data - sme neodovzdali, nezmestili by sa nám do adresára, môžete ich
		    nájsť tu:
https://drive.google.com/file/d/1G8nge-WMcNCTx-x7Rdj0Hh8aYfu8P0kL/view?usp=sharing

Zoznam a popis rôznych prístupov, ktoré sme použili:
--------------------------------------------------------------------------------
1. cnn
================================================================================
- Vlastni neuronova sit napsana v Pythonu s vyuzitim Tensorflow.
- Bylo trenovano spoustu ruznych modelu na augmentovanych trenovacich datech.
- Na zaklade trenovani vysel nejlepe model s:
    - 1x konvolucni vrstvou o 128 neurononech (+ max pooling)
    - 2x plne propojenou vrstvou o 512 neuronech
    - 1x dropout vrstvou s pravdepodobnosti 0.35
    - Nad datasetem bylo provedeno 10 iterací

--------------------------------------------------------------------------------
2. pretrained resnet50 finetuned - fixed feature extractor
   file with results: - resnet50_finetuned_result.txt
   python file downloaded from google colab - finetuned_resnet50.py
================================================================================
- Predtrenovaná sieť resnet50, zafixovaná celá sieť okrem poslednej vrstvy,
  tzn. len parametre poslednej vrstvy sa počas trénovania učia, 
  naprogramované v jazyku Python s využitím frameworku Pytorch, cely vývoj 
  prebiehal na google colaboratory
- Sieť dosahuje na evaluačnej sade úspešnosť ~95%, resnet18 trenovaná rovnakým 
  sposobom dosahovala úspešnosť ~94%. Použitie data augmentácie takmer vôbec
  nevylepšilo výsledok rozpoznávania na evaluačnej sade obrázkov.

--------------------------------------------------------------------------------
3. Gausian mixture model
   file with results: - gmm_speech.txt
================================================================================
- Gausian mixture model pro rozpoznávání dle hlasových nahrávek
- Metoda trénování - Expectation maximization
- Na zaklade trenovani vysel nejlepe model s:
    - počtem gausovských rozložení = 8
    - Nad datasetem bylo provedeno 110 iterací

--------------------------------------------------------------------------------
4. Finetuned ResNet 50 pretrained on VggFace2 dataset
   file with results: resnet50_vggface2_pretrained.txt
   src dir: SRC\resnet50_vggface2_pretrained
   reproduction steps in SRC\resnet50_vggface2_pretrained\README
   colab link: 
     https://colab.research.google.com/drive/1Y9TrGEIdcTqO_WNvE0JY4_ekunPekh_L
   dataset project link:
     http://www.robots.ox.ac.uk/~vgg/data/vgg_face2/
   used model link:
	https://github.com/ox-vgg/vgg_face2 model ResNet-50
================================================================================
- Predtrénovaná ResNet50 na datasete VGGFace2 s plibližne 8000 rôznymi triedami
  klasifikácie. Použitý bol výstup predposlednej vrstvy s 2048 features, na kt.
  bola pripojená a dotrénovaná lineárna vrstva s 2 výstupmi. Vstup ResNet 50 je 
  obvykle o veľkosti 3x224x224. Keďže naše vstupy sú veľkosti 80x80, bol zmenený
  stride prvej konvolučnej vrstvy z 2 na 1, čím sa zredukoval vstup siete na 
  3x112x112, a zároveň tak boli zachované predtrénované parametre konvolučných 
  filtrov. Zväčšenie na 112x112 je realizované knižnicou PIL použitím 
  bilineárneho filtra.
- Bol taktiež realizovaný pokus o použitie menšieho modelu Squzee-Excitation
  ResNet50 `SE-ResNet-50-128D` embedujúceho do 128 dimenzionálneho feature 
  priestoru. Ten však nebolo možné natrénovať na našej malej trénovacej sade.
