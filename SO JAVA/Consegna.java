import java.util.Random;

public class Consegna {
    
    final static int SO_NAVI = 3; // numero di navi
    final static int SO_PORTI = 4; // numero porti 
    final static int SO_BANCHINE = 2; // numero di banchine per porti
    
    final static double SO_CAPACITY = 10; // capienza massima (in ton) per ogni nave
    final static double SO_SPEED = 500; // velocità (in km/giorno) per ogni nave
    final static double SO_LOADSPEED = 200; // velocità (in ton/giorno) per ogni nava per lo scaricamento e caricamento merci
    
    final static int SO_MERCI = 3; // numero di tipo di merci
    final static int SO_SIZE = 61; // numero massimo (in ton) per un determinato tipo di merce
    final static int SO_FILL = 500000; // numero (in ton) totale per tutte le merci
    final static int SO_MIN_VITA = 15; // numero (in giorni) minimo di durata di una merce - scadenza
    final static int SO_MAX_VITA = 26; // numero (in giorni) massimo di durata di una determinata merce - scandenza
    
    final static int SO_DAYS = 10; // numero di giorni
    final static double SO_LATO = 1000; // misura (in km) del lato della mappa
    
    // CREAZIONE DI SO_PORTI di cui i primi 4 sono agli angoli della mappa, mentre gli altri in posizione randomica
    static Porto[] porti = crea_porti();
    // CREAZIONE DI SO_NAVI in posizione randomica
    static Nave[] navi = crea_navi();
    // CREAZIONE DI SO_MERCI 
    static Merce[] merci = crea_merci();
    // CREAZIONE DELLE OFFERTE 
    
    public static void main(String[] args) {    
        smista_merci();
        Offerta[] offerte = crea_offerte();
        Richiesta[] richieste = crea_richieste(); 


    }

    //FUNZIONE PER CREARE L'ARRAY DI PORTI  
    public static Porto[] crea_porti(){
        System.out.println("--------------- CREAZIONE PORTI ---------------");
        Porto[] porti = new Porto[SO_PORTI];
        for (int j = 0; j < SO_PORTI; j++)
        if (j == 0)
        porti[0] = new Porto(0, 0);
            else if (j == 1)
                porti[1] = new Porto(0, SO_LATO);
            else if (j == 2)
                porti[2] = new Porto(SO_LATO, SO_LATO);
            else if (j == 3)
                porti[3] = new Porto(SO_LATO, 0);
            else
            porti[j] = new Porto(Consegna.Random(SO_LATO), Consegna.Random(SO_LATO));
            
        for (int j = 0; j < SO_PORTI; j++)
        System.out.println("Posizione del porto n° " + j + ": " + porti[j]);
        System.out.println();
        return porti; 
    }
    
    //FUNZIONE PER CREARE L'ARRAY DI NAVI
    public static Nave[] crea_navi(){
        System.out.println("--------------- CREAZIONE NAVI ---------------");
        Nave[] navi = new Nave[SO_NAVI];
        for (int i = 0; i < SO_NAVI; i++)
        navi[i] = new Nave(Consegna.Random(SO_LATO+1), Consegna.Random(SO_LATO+1));
        
        for (int i = 0; i < SO_NAVI; i++)
            System.out.println("Posizione della nave n° " + i + ": " + navi[i]);
        
        System.out.println();
        return navi;
    }

    //FUNZIONE PER CREARE L'ARRAY DI MERCI
    public static Merce[] crea_merci(){
        System.out.println("--------------- CREAZIONE MERCI ---------------");
        Merce[] a = new Merce[SO_MERCI];
        for(int i=0; i<SO_MERCI; i++){
            a[i] = new Merce(i);
            System.out.println(a[i]);
        }
        System.out.println();
        return a;
    }
    
    //FUNZIONE PER CREARE L'ARRAY DI OFFERTE 
    public static Offerta[] crea_offerte(){ 
        System.out.println("--------------- CREAZIONE OFFERTE ---------------");     
        Offerta[] offerte = new Offerta[contatore_offerte()]; 
        int k=0; 
        for(int i=0; i<SO_PORTI; i++)
            for(int j=0; j<SO_MERCI; j++)
                if(porti[i].lotti[j] > 0){
                    offerte[k] = new Offerta(j, i, porti[i].lotti[j]);
                    System.out.println("L'offerta n°" + k + ":" + offerte[k]);
                    k++; 
                }
        return offerte;
    }

    //FUNZIONE CHE CALCOLA LE OFFERTE DISPONIBILE NEI VARI PORTI 
    public static int contatore_offerte(){
        int cont = 0;
        for(int i=0; i<SO_PORTI; i++){
            for(int j=0; j<SO_MERCI; j++){
                if(porti[i].lotti[j] > 0){
                    cont++;      
                }
            }
        }     
        return cont; 
    }

    //FUNZIONE PER CREARE L'ARRAY DI RICHIESTE
    public static Richiesta[] crea_richieste(){
        System.out.println("\n--------------- CREAZIONE RICHIESTE ---------------");   
        int merce_piazzata = 0; 
        int cont = 0;
        Richiesta[] richieste = null; 
        Richiesta[] temp = null;
        while(merce_piazzata <= SO_FILL && altre_richieste(merce_piazzata)){   //PROBLEMA: devo sforzare come prima di ricalcolare finchè si generi al più SO_FILL(?)
            int ind_porti;
            int[] merci_possibili;
            int ind_merce;
            int lotti;
            do{
                //estrattore casuale di indice porti 
                ind_porti = (int) Random(SO_PORTI);
                //estrattore casuale di indice merci fra gli indici proposti da trova_merci_richieste()
                merci_possibili = trova_merci_richieste(ind_porti, merce_piazzata);
            }while(merci_possibili.length == 0 && altre_richieste(merce_piazzata));

            do{
                int m = (int) Random(merci_possibili.length);
                ind_merce = merci_possibili[m];
                //estrattore casuale di lotti dalle tonnellate diponibili
                lotti = (int) Random(((SO_FILL-merce_piazzata)/merci[ind_merce].quantità)+1);
            }while(lotti == 0);
            
            cont++; 
            richieste = new Richiesta[cont];
            if(cont==1){
                richieste[0] = new Richiesta(ind_merce, ind_porti, lotti);
            }else{ 
                int i; 
                for(i=0; i<temp.length; i++)
                    richieste[i] = temp[i]; 
                //i++; 
                richieste[i] = new Richiesta(ind_merce, ind_porti, lotti);
            }

            temp = richieste;
            //incrementiamo la merce piazzata
            merce_piazzata += lotti*merci[ind_merce].quantità;
        }
        System.out.println("Merce richiesta totale = " + merce_piazzata);
        stampa_richieste(richieste);
        ordina_richieste(richieste);
        System.out.println();
        stampa_richieste(richieste);
        return richieste; 
    }

    //FUNZIONE CHE MI TORNA SE CI STANNO ALTRE MERCI OPPURE NO
    public static boolean altre_richieste(int merce_piazzata){
        boolean cond = false; 
        for(int i=0; i<SO_MERCI && !cond; i++)
            if(merce_piazzata + merci[i].quantità <= SO_FILL && assente_porto(i)){
                cond = true;
            }
        return cond; 
    }

    public static boolean assente_porto(int ind_merce){
        boolean cond = false; 
        for(int i=0; i<SO_PORTI && !cond; i++)
            if(porti[i].lotti[ind_merce] == 0)
                cond = true; 
        return cond;
    }

    //FUNZIONE CHE MI STAMPA L'ARRAY RICHIESTE
    public static void stampa_richieste(Richiesta[] richieste){
        for(int i=0; i<richieste.length; i++){
            System.out.println("RICHIESTA n°" + i + richieste[i]);
        }
    }

    //FUNZIONE CHE CREI UN ARRAY CON ALL'INTERNO GLI INDICI CON LE MERCI POSSIBILI
    public static int[] trova_merci_richieste(int ind_porti, int merce_piazzata){
        //creare un array di lunghezza = merci possibili 
        int cont = 0;
        for(int i=0; i<SO_MERCI; i++)
            if(porti[ind_porti].lotti[i] == 0 && (SO_FILL-merce_piazzata-merci[i].quantità)>=0)
                cont++; 
        int[] indici = new int[cont];
                
        int k=0; 
        for(int j=0; j<SO_MERCI; j++){
            if(porti[ind_porti].lotti[j] == 0 && (SO_FILL-merce_piazzata-merci[j].quantità)>=0){
                indici[k] = j;         
                k++;
            }
        }
        return indici; 
    }

    //FUNZIONE CHE MI ORDINA L'ARRAY DI RICHIESTE
    public static void ordina_richieste(Richiesta[] richieste){
        //ordinamento e votazione per data di scadenza 
        ordina_richieste_scadenza(richieste);
        //ordinamento e votazione per quantità
        ordina_richieste_quantità(richieste); 

    }

    //FUNZIONE CHE VALUTA L'ARRAY DI RICHIESTE PER DATA DI SCADENZA
    public static void ordina_richieste_scadenza(Richiesta[] richieste){
        //ordinamento dell'array 
        Richiesta temp; 
        for(int i=0; i<richieste.length; i++){
            int ind_merce_min = richieste[i].ind_merce;
            int min_vita = merci[ind_merce_min].SO_VITA;
            for(int j=i+1; j<richieste.length; j++){
                int ind_merce = richieste[j].ind_merce; 
                if(merci[ind_merce].SO_VITA < min_vita){
                    temp = richieste[i]; 
                    richieste[i] = richieste[j]; 
                    richieste[j] = temp; 
                    min_vita = merci[ind_merce].SO_VITA;
                }
            }
        } 
        //votazione scadenze 
        int voto = richieste.length; 
        for(int i=0; i<richieste.length; i++){
            richieste[i].priorità += voto;
            voto--; 
        }
    }

    //FUNZIONE CHE VALUTA L'ARRAY DI RICHIESTE PER QUANTITA'
    public static void ordina_richieste_quantità(Richiesta[] richieste){
        //ordina array 
        Richiesta temp; 
        for(int i=0; i<richieste.length; i++){
            int max = richieste[i].lotti;
            for(int j=i+1; j<richieste.length; j++){
                if(richieste[j].lotti > max){
                    temp = richieste[i]; 
                    richieste[i] = richieste[j]; 
                    richieste[j] = temp; 
                    max = richieste[j].lotti;
                }
            }
        } 
        //votazione scadenze 
        int voto = richieste.length; 
        for(int i=0; i<richieste.length; i++){
            richieste[i].priorità += voto;
            voto--; 
        }
    }

    //FUNZIONE CHE CALCOLA L'INDICE DELLA NAVE CHE IMPIEGA MENO TEMPO A PERCORRERE UNA DATA TRATTA
    public static int scelta_nave(int A, int B){
        double[] tempi_navi = new double[SO_NAVI];
        for (int i=0; i<SO_NAVI; i++) {
            tempi_navi[i] = t_tot(i, A, B);
        }
        return min_t(tempi_navi);
    }
    
    //FUNZIONE CHE CALCOLA L'INDICE DELLA NAVE CHE IMPIEGA MENO TEMPO A PERCORRERE UNA DATA TRATTA
    public static int min_t(double[] a){
        double min = a[0]; 
        int indice = 0; 
        for(int i=1; i<SO_NAVI; i++)
            if(a[i]<min){
                min = a[i];
                indice = i;
            }
        return indice;
    }
    
    //FUNZIONE PER CREARE IL PIANO CARTESIANO PER LA MAPPA
    public static void mappa(Porto[] porti, Nave[] navi) {
        for (int i = (int) SO_LATO / 100; i >= 0; i--) {
            System.out.print(i + "    ");
            for (int j = 0; j <= SO_LATO / 100; j++)
            System.out.print("|  ");
            System.out.println();
        }

        System.out.print("      ");
        for (int j = 0; j <= SO_LATO / 100; j++)
            System.out.print(j + "  ");
    }
    
    //FUNZIONE CHE ESTRAE CASUALMENTE UN NUMERO
    public static double Random(double m) {
        Random casuale = new Random();
        double n = casuale.nextDouble(m);
        return n;
    }

    //FUNZIONE CHE CALCOLA LA DISTANZA FRA DUE PUNTI
    public static double distance(double x1, double y1, double x2, double y2) {
        return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
    }

    //FUNZIONE CHE CALCOLA IL TEMPO IMPIEGATO PER PERCORRE UNA DATA DISTANZA
    public static double t_viaggio(double distanza) {
        return distanza / SO_SPEED;
    }

    //FUNZIONE CHE CALCOLI IL TEMPO IMPIEGATO PER CARICARE/SCARICARE UNA DATA QUANTITA' DI MERCE
    public static double t_load(double ton) {
        return ton / SO_LOADSPEED;
    }

    //FUNZIONE CHE CALCOLI IL TEMPO IMPIEGATO PER COMPIERE UN'INTERA TRATTA
    public static double t_tot(int N, int A, int B) {
        double d1 = distance(navi[N].posizione_x, navi[N].posizione_y, porti[A].posizione_x, porti[A].posizione_y);
        double t1 = t_viaggio(d1);
        double t2 = t_load(SO_CAPACITY);
        double d2 = distance(porti[A].posizione_x, porti[A].posizione_y, porti[B].posizione_x, porti[B].posizione_y);
        double t3 = t_viaggio(d2);
        return (t1 + t2 + t3 + t2);
    }

    //FUNZIONE CHE MI SMISTA LE SO_FILL MERCI NEI VARI PORTI
    public static void smista_merci(){

        int merce_piazzata = 0; //numero di merce portata nei porti
        int[] merci_possibili = trova_merci(merce_piazzata); 

        while((merci_possibili.length != 0) && (merci_possibili.length <= SO_MERCI) && (merce_piazzata <= SO_FILL)){
            
            //estrattore casuale di indice porti 
            int ind_porti = (int) Random(SO_PORTI);
            //estrattore casuale di indice merci fra gli indici proposti da trova_merci()
            int m = (int) Random(merci_possibili.length);
            int ind_merce = merci_possibili[m];
            //estrattore casuale di lotti dalle tonnellate diponibili
            int lotti = (int) Random(((SO_FILL-merce_piazzata)/merci[ind_merce].quantità)+1);
            //scrivo la quantità di lotti trovata all'interno del porto
            porti[ind_porti].lotti[ind_merce] += lotti;
            //incrementiamo la merce piazzata
            merce_piazzata += lotti*merci[ind_merce].quantità; 
            merci_possibili = trova_merci(merce_piazzata);
        }
        stampa_merci_porti(merce_piazzata); 
    }
    
    //FUNZIONE CHE MI CREA L'ARREY DI INDICI DELLE MERCI POSSIBILI
    public static int[] trova_merci(int merce_piazzata){
        //creare un array di lunghezza = merci possibili 
        int cont = 0;
        for(int i=0; i<SO_MERCI; i++)
            if((SO_FILL - merce_piazzata)-merci[i].quantità >= 0)
                cont++; 
        int[] indici = new int[cont];
                
        int k=0; 
        for(int j=0; j<SO_MERCI; j++){
            if((SO_FILL - merce_piazzata)-merci[j].quantità >= 0){
                indici[k] = j;         
                k++;
            }
        }
        
        return indici; 
    }
    
    //FUNZIONE CHE STAMPI LE QUANTITA' DI LOTTI PRESENTI NEI PORTI 
    public static void stampa_merci_porti(int merce_piazzata){
        System.out.println("--------------- SMISTAMENTO MERCI ---------------");
        System.out.println("SO_FILL = " + SO_FILL);
        System.out.println("Merce piazzata = : " + merce_piazzata);
        for(int i=0; i<SO_PORTI; i++){
            System.out.println("Nel porto n°" + i + " sono presenti le seguenti merci");
            for(int j=0; j<SO_MERCI; j++)
                System.out.println("  # Merce tipo [" + j + "]: " + porti[i].lotti[j] +  " lotti (= " + (porti[i].lotti[j]*merci[j].quantità) + ") ton.");
            System.out.println();
        }
    }

}

/*
    Creare un algoritmo che data una richiesta scelga quale sia il porto da cui caricare (in base all'offerta, quantità, data di scadenza 
    per il tipo di merce richiesto per ogni porto) crei il percorso migliore (vd. Google Maps) e poi il secondo algoritmo confronti la posizione 
    di ogni nave DISPONIBILE per valutare quale sia la nave migliore. 
    Nota: le task - offerte devono essere inserite in un array gerarchico che si aggiorni per togliere ogni richiesta presa in carico e per
    far salire di importanza merci che sono pronti allo scadere. 
*/

// fare tipo una super, oppure n argomenti per n porti in cui andare. 

//NOTA: gestire problema di due porti sovrapposti (anche quelli ai 4 angoli)


/* 
La lista di richieste è ordinata in base alla scadenza, e in caso di parità, alla quantità.
Una volta creata la lista ci si deve occupare di soddisfare la prima richiesta: chiamando le navi che impiegano meno tempo (scegliendo il 
percorso migliore). 
Una richiesta esce dalla lista se: 
    - è stata soddisfatta 
    - non si può più soddisfare, questo accede quando: 
                    a) la merce scadrebbe prima che arrivi al porto di scarico
                    b) non c'è più offerta per soddisfare la richiesta
*/

/* 
    -------- GESTIONE DELLE RICHIESTE -------- 

    Le richieste hanno i seguenti attributi: 
        - (int) numero identificativo della richiesta 
        - (int) priorità 
        - (int) indice tipo merce 
        - (int) indice porto richiedente 
        - (int) quantità di lotti richiesti
    
    Le richieste vengono inserite in una lista di richieste: 
        All'inizio della simulazione la situazione è la seguente:   NULL
        Al primo passo della simulazione:                           |RICHIESTA 1|-->NULL
        Al secondo passo della simulazione:                         |RICHIESTA 2|-->|RICHIESTA 1|-->NULL
        Al n-esimo passo della simulazione:                         |RICHIESTA n|--> ... -->|RICHIESTA 2|-->|RICHIESTA 1|-->NULL 

    Si generano le richieste in maniera casuale: 
        0. CICLO con condizione merce_piazzata <= SO_FILL
        1. si estrae casualmente un indice di merce (ind_merce) compreso fra 0 e SO_MERCI.
        2. attraverso una funzione trova_porto_richieste(), ci verrà ritornato un array (porti_possibili) di lunghezza pari ai 
            porti in cui non vi sono lotti della merce di ind_merce precedentemente estratto e con all'interno l'indice del porto 
            in cui non c'è la merce.
        3.1 se la lunghezza dell'array è uguale di zero, allora ritorna al punto 1. 
        3.2 altrimenti estrai casualmente un numero compreso fra 0 e porti_possibili.length e inseriscilo nella variabile ind_porti. 
        4. estraiamo un intero di casualmente da 1 a ((SO_FILL-merce_piazzata)/quantità per ind_merce)+1, ossia estraiamo i lotti 
            (di tonnellate al più uguali SO_FILL)
        5. possiamo dunque assegnare indice tipo merce, indice tipo porto richiedente, quantità di lotti richiesti e numero indentificativo(?)
        6. incrementiamo merce_piazzata e richiamo la funzione trova_porto_richieste(). 

    Viene chiamata una funzione ordina_richieste1() che si occupa di ordinare la lista sulla base della DATA DI SCADENZA 
    e sulla base della QUANTITA' DI MERCE RICHIESTA. 
        # contatore di lista che mi dice quante richieste attive ho 
        # algoritmo per l'ordinamento della lista in base alla data di scadenza della merce indicata dall'indice tipo merce
            [] 
            []
            []
        # sommare (attraverso il contatore precedentemente usato) al valore di priorità il valore del voto per ciascuna richiesta
        # algoritmo per l'ordinamento della lista in base alla data di scadenza della merce indicata dall'indice tipo merce 
            []
            []
            []
        # sommare (attraverso il contatore precedentemente usato) al valore di priorità il valore del voto per ciascuna richiesta 
        # algoritmo per l'ordinamento della lista in base alla priorità
            []
            []
            []
        
    Viene calcolato il tempo minore possibile per consegnare il prodotto. 
        0.1 fa una copia dell'array porto
        0.2 creare un array di length SO_PORTI-1 in cui inserire le priorità
        1. si cerca il porto da cui prendere la merce: 
            a) algoritmo per l'ordinamento dell'array in base alla distanza dal porto di scarico 
                (NOTA: non calcorare il la distanza dal porto stesso)
            b) sommare (attraverso SO_PORTI) al valore di priorità il valore del voto per ciascun porto  
                (NOTA: non calcorare la priorità per il porto stesso??)
            
            
                
            
                 


        
    






*/