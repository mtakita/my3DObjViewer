#pragma once
namespace jpeglib {
	extern "C" {

#include <jpeglib.h>

		//		extern "C" struct jpeg_error_mgr * jpeg_std_error(struct jpeg_error_mgr * err);

	}
}

namespace jpeglib {

	struct my_error_mgr {
		struct jpeg_error_mgr pub;

		jmp_buf setjmp_buffer;
	};

	typedef struct my_error_mgr* my_error_ptr;

	METHODDEF(void)
		my_error_exit(j_common_ptr cinfo)
	{
		my_error_ptr myerr = (my_error_ptr)cinfo->err;

		(*cinfo->err->output_message) (cinfo);
		longjmp(myerr->setjmp_buffer, 1);
	}

	//		extern "C" struct jpeg_error_mgr * __cdecl jpeg_std_error JPP((struct jpeg_error_mgr * err));

}

