	.module tilemap

    .globl _UpdateTransitionVblank

_UpdateTransitionVblank:
	ld	a, (#_gDoorTransition)
	or	a, a
    ret Z

	ld	a, (#_gDoorTransition)
    dec a
    jr  Z, UpdateNormalTransition

	ld	a, (#_gDoorTransition)
    sub a, #0x02
    jr  Z, UpdateLoadingTransition

UpdateNormalTransition:
    ret

UpdateLoadingTransition:
    ret
