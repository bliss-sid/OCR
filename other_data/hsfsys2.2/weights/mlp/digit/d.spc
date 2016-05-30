/*************************************/
/*          training run 0           */
/*************************************/
train_or_test		train
purpose			classifier
seed			12347

/* architecture parameters */
ninps			128
nhids			128
nouts			10
errfunc			mse
acfunc_hids		sinusoid
acfunc_outs		sinusoid
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			2.0
patterns_infile 	h6_d.pat
npats			61094
wts_outfile		trn_0.wts
short_outfile		trn_0.err

newrun
/*************************************/
/*          testing run 0            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			2.0
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_0.wts
short_outfile		tst_0.err

newrun
/*************************************/
/*          training run 1           */
/*************************************/
train_or_test		train

/* architecture parameters */
errfunc			mse
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			1.0
patterns_infile 	h6_d.pat
npats			61094
wts_infile		trn_0.wts
wts_outfile		trn_1.wts
short_outfile		trn_1.err

newrun
/*************************************/
/*          testing run 1            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			1.0
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_1.wts
short_outfile		tst_1.err

newrun
/*************************************/
/*          training run 2           */
/*************************************/
train_or_test		train

/* architecture parameters */
errfunc			mse
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			0.5
patterns_infile 	h6_d.pat
npats			61094
wts_infile		trn_1.wts
wts_outfile		trn_2.wts
short_outfile		trn_2.err

newrun
/*************************************/
/*          testing run 2            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			0.5
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_2.wts
short_outfile		tst_2.err

newrun
/*************************************/
/*          training run 3           */
/*************************************/
train_or_test		train

/* architecture parameters */
errfunc			mse
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			0.2
patterns_infile 	h6_d.pat
npats			61094
wts_infile		trn_2.wts
wts_outfile		trn_3.wts
short_outfile		trn_3.err

newrun
/*************************************/
/*          testing run 3            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			0.2
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_3.wts
short_outfile		tst_3.err

newrun
/*************************************/
/*          training run 4           */
/*************************************/
train_or_test		train

/* architecture parameters */
errfunc			mse
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			0.1
patterns_infile 	h6_d.pat
npats			61094
wts_infile		trn_3.wts
wts_outfile		trn_4.wts
short_outfile		trn_4.err

newrun
/*************************************/
/*          testing run 4            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			0.1
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_4.wts
short_outfile		tst_4.err

newrun
/*************************************/
/*          training run 5           */
/*************************************/
train_or_test		train

/* architecture parameters */
errfunc			mse
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			0.01
patterns_infile 	h6_d.pat
npats			61094
wts_infile		trn_4.wts
wts_outfile		trn_5.wts
short_outfile		trn_5.err

newrun
/*************************************/
/*          testing run 5            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			0.01
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_5.wts
short_outfile		tst_5.err

newrun
/*************************************/
/*          training run 6           */
/*************************************/
train_or_test		train

/* architecture parameters */
errfunc			mse
priors			allsame
boltzmann		square_prune
temperature 		1.0e-4
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* iteration control */
niter_max		50
nfreq			10

/* stopping conditions */
egoal			0.000
gwgoal			0.0
errdel			0.99
nokdel			1

/* changing parameters across runs */
regfac			0.001
patterns_infile 	h6_d.pat
npats			61094
wts_infile		trn_5.wts
wts_outfile		trn_6.wts
short_outfile		trn_6.err

newrun
/*************************************/
/*          testing run 6            */
/*************************************/
train_or_test		test

/* architecture parameters */
errfunc			mse
priors			allsame
oklvl			0.0
trgoff			0.0

/* inputs and attributes */
lcn_scn_infile		d.scn
patsfile_ascii_or_binary binary

/* output options */
do_confuse		true
do_cvr			true

/* changing parameters across runs */
regfac			0.001
patterns_infile		h7_d.pat
npats			60089
wts_infile		trn_6.wts
short_outfile		tst_6.err
