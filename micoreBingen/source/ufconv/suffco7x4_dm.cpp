#include"../../include/gfxcoder.h"

void gfxcoder::suffco7x4_dm( uint32_t mask )
{
    uint32_t c[4][4], a[2][4], b[2][4];
    for( uint32_t i=0; i<16; ++i ){ ((uint32_t*)&c[0][0])[i]=v0+i; }
    for( uint32_t i=0; i<4; ++i ){
        a[0][i]=v16+i; b[0][i]=v20+i; 
        a[1][i]=v24+i; b[1][i]=v28+i;
    }

    const uint32_t argptr=s0 ;
    const uint32_t gidx  =s3 ;
    const uint32_t gidy  =s2 ;
    const uint32_t gidz  =s4 ;
    const uint32_t igroup=s21;
    const uint32_t abase =s4 ;
    const uint32_t bbase =s6 ;
    const uint32_t ng    =s8 ;
    const uint32_t m     =s9 ;
    const uint32_t n     =s10;
    const uint32_t k     =s11;
    const uint32_t amag  =s12;
    const uint32_t cmag  =s14;
    const uint32_t dimx  =s19;
    const uint32_t col   =s20;
    const uint32_t dbtr  =s8 ;
    const uint32_t elda  =s2 ;
    const uint32_t incr  =s3 ;
    const uint32_t cbase =s0 ;
    const uint32_t alpha =s2 ;

    const uint32_t apld=v32;
    const uint32_t bpld=v40;
    const uint32_t ao  =v41;
    const uint32_t bo  =v45;
    const uint32_t co  =v46;
    const uint32_t asld=v47;
    const uint32_t bsld=v48;
    const uint32_t asst=v49;
    const uint32_t bsst=v50;
    const uint32_t csst=v51;
    const uint32_t csld=v52;
    const uint32_t tidx=v53;

    metadata_start();
    {
        metadata_set_lds_size(0x2600);
        metadata_set_kernarg_align(5);
        metadata_set_kernarg_size(64);
        metadata_set_group_size(128);
        metadata_set_sgprcnt(28);
        metadata_set_vgprcnt(54);
    }
    metadata_end();

    kernel.start_descr();
    {
        kernel.enable_kernarg_segment();
        kernel.set_fp32_mode_round(amdgpu_fp_mode_rn);
        kernel.set_fp32_mode_denorm(amdgpu_fp_denorm_fsd);
        kernel.set_fp16_fp64_mode_round(amdgpu_fp_mode_rn);
        kernel.set_fp16_fp64_mode_denorm(amdgpu_fp_denorm_fn);
        kernel.enable_dx10_clamp();
        kernel.enable_ieee_mode();
        kernel.set_user_sgprcnt(2);
        kernel.enable_sgpr_group_id_x();
        kernel.enable_sgpr_group_id_y();
        kernel.enable_sgpr_group_id_z();
        kernel.enable_vgpr_iterm_id_1d();
    }
    kernel.end_descr();

    s_mov_b32(igroup,gidz);
    s_shl_b32(col,gidy,4);
    s_lda_b256(s4,argptr,0x00);
    s_lda_b128(s12,argptr,0x20);
    s_lda_b32(dimx,argptr,0x3c);
    v_and_b32(v1,ic(63),v0);
    v_shl_or_b32(tidx,gidx,7,v0);
    v_shl_or_b32(v3,gidx,6,v1);
    v_shr_b32(v2,v0,6);
    s_sync(lgkmcnt(0));

    s_mul_i32(s22,k,ng);
    s_mul_i32(s23,n,ng);
    UFCO_COMPUTE_START_ADDR_A(ao,m,s22,v3,dimx,amag,v4,s16,1);
    v_shr_b32(v3,v0,3);
    v_and_b32(v4,ic(7),v0);
    v_or_b32(v5,col,v3);
    s_mul_i32(s16,k,igroup);
    s_movk_i32(s17,0);
    S_MUL_U64_U32(s12,m,s16);
    s_shl_b64(s12,s12,2);
    s_shl_b64(s16,s16,2);
    v_sub_i32(v6,n,ic(1));
    S_ADD_U64(abase,abase,s12);
    S_ADD_U64(bbase,bbase,s16);
    v_cmp_u32(n,v5,gt);
    v_cmov_b32(v5,v6,v5);
    s_shl_b32(s2,m,3);
    v_mul_lo_u32(v7,m,v2);
    v_mul_lo_u32(v8,s22,v5);
    v_shl_b32(v9,v4,2);
    v_shl_add_u32(ao,v7,2,ao);
    v_shl_add_u32(bo,v8,2,v9);
    v_add_u32(ao+1,s2,ao+0);
    v_add_u32(ao+2,s2,ao+1);
    v_add_u32(ao+3,s2,ao+2);
    ldg_b64(apld+0,abase,ao+0,0);
    ldg_b64(apld+2,abase,ao+1,0);
    ldg_b64(apld+4,abase,ao+2,0);
    ldg_b64(apld+6,abase,ao+3,0);
    ldg_b32(bpld+0,bbase,bo+0,0);
    v_readlane0_b32(s2,v2);
    UFCO_COMPUTE_START_ADDR_C(co,m,s23,tidx,cmag,v5,s16,0);

    //v5=v1&15
    //v6=v1>>4
    //asld=(wave<<8)|(v5<<4)
    //bsld=v6<<4
    //asst=v0<<3
    //bsst=(v4<<6)|(v3<<2)
    //csst=(wave<<10)|(v6<<8)|(v5<<4)
    //csld=(wave<<10)|(v1<<2)
    s_shl_b32(s3,s2,10);
    v_and_b32(v5,ic(15),v1);
    v_shr_b32(v6,v1,4);
    v_shl_b32(v7,v5,4);
    v_shl_b32(v8,v3,2);
    v_shl_b32(bsld,v6,4);
    v_shl_b32(asst,v0,3);
    v_shl_or_b32(v9,v6,8,v7);
    v_shl_or_b32(asld,s2,8,v7);
    v_shl_or_b32(bsst,v4,6,v8);
    v_shl_or_b32(csld,v1,2,s3);
    v_or_b32(csst,s3,v9);

    for( int i=0; i<16; ++i ){ v_mov_b32(((uint32_t*)&c[0][0])[i],zero); }
    s_sync(vmcnt(0)&lgkmcnt(0));
    lsstx_b64x2(asst,0,apld+0,2,apld+2);
    lsstx_b64x2(asst,4,apld+4,6,apld+6);
    lsst_b32(bsst,0x1000,bpld);
    s_shl_b32(elda,m,5);
    s_sub_u32(k,k,ic(8));
    s_movk_i32(incr,32);
    s_cmpk_u32(k,0,eq);
    s_movk_i32(dbtr,0x1400);
    s_cmovk_i32(elda,0);
    s_cmovk_i32(incr,0);
    s_sync(lgkmcnt(0));
    s_barrier();
    lsld_b128(a[0][0],asld,0x0000);
    lsld_b128(b[0][0],bsld,0x1000);
    S_ADD_U64_U32(abase,abase,elda);
    S_ADD_U64_U32(bbase,bbase,incr);

__lable__(0);
    lsld_b128(a[1][0],asld,0x0200);
    lsld_b128(b[1][0],bsld,0x1040);
    v_xor_b32(asst,dbtr,asst);
    s_sync(lgkmcnt(2));
    ldg_b64(apld+0,abase,ao+0,0);
    ldg_b64(apld+2,abase,ao+1,0);
    ldg_b64(apld+4,abase,ao+2,0);
    ldg_b64(apld+6,abase,ao+3,0);
    ldg_b32(bpld+0,bbase,bo+0,0);
    V_OP4_R(&c[0][0],&a[0][0],b[0][0]);
    V_OP4_S(&c[1][0],&a[0][0],b[0][1]);
    V_OP4_S(&c[2][0],&a[0][0],b[0][2]);
    V_OP4_T(&c[3][0],&a[0][0],b[0][3]);
    lsld_b128(a[0][0],asld,0x0400);
    lsld_b128(b[0][0],bsld,0x1080);
    v_xor_b32(bsst,dbtr,bsst);
    s_sync(lgkmcnt(2));
    V_OP4_R(&c[0][0],&a[1][0],b[1][0]);
    V_OP4_S(&c[1][0],&a[1][0],b[1][1]);
    V_OP4_S(&c[2][0],&a[1][0],b[1][2]);
    V_OP4_T(&c[3][0],&a[1][0],b[1][3]);
    lsld_b128(a[1][0],asld,0x0600);
    lsld_b128(b[1][0],bsld,0x10c0);
    s_cmpk_u32(k,8,eq);
    s_sync(lgkmcnt(2));
    V_OP4_R(&c[0][0],&a[0][0],b[0][0]);
    V_OP4_S(&c[1][0],&a[0][0],b[0][1]);
    V_OP4_S(&c[2][0],&a[0][0],b[0][2]);
    V_OP4_T(&c[3][0],&a[0][0],b[0][3]);
    lsld_b128(a[0][0],asld,0x0800);
    lsld_b128(b[0][0],bsld,0x1100);
    s_cmovk_i32(elda,0);
    s_cmovk_i32(incr,0);
    s_sync(lgkmcnt(2));
    V_OP4_R(&c[0][0],&a[1][0],b[1][0]);
    V_OP4_S(&c[1][0],&a[1][0],b[1][1]);
    V_OP4_S(&c[2][0],&a[1][0],b[1][2]);
    V_OP4_T(&c[3][0],&a[1][0],b[1][3]);
    lsld_b128(a[1][0],asld,0x0a00);
    lsld_b128(b[1][0],bsld,0x1140);
    S_ADD_U64_U32(abase,abase,elda);
    s_sync(lgkmcnt(2));
    V_OP4_R(&c[0][0],&a[0][0],b[0][0]);
    V_OP4_S(&c[1][0],&a[0][0],b[0][1]);
    V_OP4_S(&c[2][0],&a[0][0],b[0][2]);
    V_OP4_T(&c[3][0],&a[0][0],b[0][3]);
    lsld_b128(a[0][0],asld,0x0c00);
    lsld_b128(b[0][0],bsld,0x1180);
    S_ADD_U64_U32(bbase,bbase,incr);
    s_sync(lgkmcnt(2));
    V_OP4_R(&c[0][0],&a[1][0],b[1][0]);
    V_OP4_S(&c[1][0],&a[1][0],b[1][1]);
    V_OP4_S(&c[2][0],&a[1][0],b[1][2]);
    V_OP4_T(&c[3][0],&a[1][0],b[1][3]);
    lsld_b128(a[1][0],asld,0x0e00);
    lsld_b128(b[1][0],bsld,0x11c0);
    s_sub_u32(k,k,ic(8));
    s_sync(vmcnt(0)&lgkmcnt(2));
    lsstx_b64x2(asst,0,apld+0,2,apld+2);
    lsstx_b64x2(asst,4,apld+4,6,apld+6);
    lsst_b32(bsst,0x1000,bpld);
    v_xor_b32(asld,dbtr,asld);
    v_xor_b32(bsld,dbtr,bsld);
    V_OP4_R(&c[0][0],&a[0][0],b[0][0]);
    V_OP4_S(&c[1][0],&a[0][0],b[0][1]);
    V_OP4_S(&c[2][0],&a[0][0],b[0][2]);
    V_OP4_T(&c[3][0],&a[0][0],b[0][3]);
    s_sync(lgkmcnt(0));
    s_barrier();
    lsld_b128(a[0][0],asld,0x0000);
    lsld_b128(b[0][0],bsld,0x1000);
    V_OP4_R(&c[0][0],&a[1][0],b[1][0]);
    V_OP4_S(&c[1][0],&a[1][0],b[1][1]);
    V_OP4_S(&c[2][0],&a[1][0],b[1][2]);
    V_OP4_T(&c[3][0],&a[1][0],b[1][3]);
    s_cbra_scc0(0);

    uint32_t is_relu=mask&0x1;
    s_lda_b32(alpha,argptr,0x38);
    s_lda_b64(cbase,argptr,0x30);
    s_mul_i32(s6,igroup,n);
    s_add_u32(s7,s6,col);
    s_shl_b32(s8,m,2);
    s_shl_b32(s9,m,4);
    S_MUL_U64_U32(s4,s7,s8);
    s_sync(lgkmcnt(0));
    S_ADD_U64(s12,cbase,s4);
    v_cmp_u32(dimx,tidx,gt);
    s_mov_b64(s4,vcc);
    s_mov_b64(s6,exec);

    for( int i=0; i<4; ++i )
    {
        if(i>0){
        __lable__(i);
            s_mov_b64(exec,s6);
            s_add_u32(col,col,ic(1));
        }
        lsst_b128(csst,0x00,c[i][0]);
        if(i==0){
            S_ADD_U64_U32(s14,s12,s9);
            S_ADD_U64_U32(s16,s14,s9);
            S_ADD_U64_U32(s18,s16,s9);
        } else {
            S_ADD_U64_U32(s12,s12,s8);
            S_ADD_U64_U32(s14,s14,s8);
            S_ADD_U64_U32(s16,s16,s8);
            S_ADD_U64_U32(s18,s18,s8);
        }
        s_mov_b64(exec,s4);
        s_sync(lgkmcnt(0));
        lsld_b32x2(v0,csld,0x00,0x40);
        lsld_b32x2(v2,csld,0x80,0xc0);

        s_cmp_u32(col,n,lt);
        s_cbra_scc0(1);
        s_sync(lgkmcnt(1));
        v_mul_f32(v0,alpha,v0);
        if(is_relu){ v_max_f32(v0,zero,v0); }
        stg_b32(s12,co,0,v0);
    
        s_add_u32(s3,col,ic(4));
        s_cmp_u32(s3,n,lt);
        s_cbra_scc0(i+1);
        v_mul_f32(v1,alpha,v1);
        if(is_relu){ v_max_f32(v1,zero,v1); }
        stg_b32(s14,co,0,v1);
        
        s_add_u32(s3,col,ic(8));
        s_cmp_u32(s3,n,lt);
        s_cbra_scc0(i+1);
        s_sync(lgkmcnt(0));
        v_mul_f32(v2,alpha,v2);
        if(is_relu){ v_max_f32(v2,zero,v2); }
        stg_b32(s16,co,0,v2);
        
        s_add_u32(s3,col,ic(12));
        s_cmp_u32(s3,n,lt);
        s_cbra_scc0(i+1);
        v_mul_f32(v3,alpha,v3);
        if(is_relu){ v_max_f32(v3,zero,v3); }
        stg_b32(s18,co,0,v3);
    }
__lable__(4);
    s_endp();
}