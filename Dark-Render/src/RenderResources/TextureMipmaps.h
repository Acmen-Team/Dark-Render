#pragma once

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace DRender::TextureMips
{
	enum class TextureMipKind
	{
		Raw,
		Color,
		Emission,
		Normal,
		Scalar,
		Roughness
	};

	struct TextureMipLevel
	{
		uint32_t _Width;
		uint32_t _Height;
		size_t _Offset;
	};

	struct TextureMipChain
	{
		std::vector<TextureMipLevel> _Levels;
		std::vector<unsigned char> _Pixels;
	};

	inline float DecodeSRGB(float channel)
	{
		return channel <= .04045f ? channel / 12.92f : std::pow((channel + .055f) / 1.055f, 2.4f);
	}

	inline float EncodeSRGB(float channel)
	{
		return channel <= .0031308f ? channel * 12.92f : 1.055f * std::pow(channel, 1.f / 2.4f) - .055f;
	}

	inline unsigned char ToUNorm8(double channel)
	{
		return static_cast<unsigned char>(std::lround(std::clamp(channel, 0., 1.) * 255));
	}

	inline TextureMipChain BuildMipChain(uint32_t width, uint32_t height, const std::vector<unsigned char>& rgba,
										 TextureMipKind kind)
	{
		if (!width || !height || uint64_t(width) * height > std::numeric_limits<size_t>::max() / 4 ||
			rgba.size() != size_t(width) * height * 4)
		{
			throw std::invalid_argument("Invalid RGBA mip sourceLevel");
		}
		TextureMipChain result{{{width, height, 0}}, rgba};
		if (kind == TextureMipKind::Raw)
		{
			return result;
		}
		if (kind == TextureMipKind::Normal)
		{
			for (size_t i = 3; i < result._Pixels.size(); i += 4)
			{
				result._Pixels[i] = 255;
			}
		}
		while (width > 1 || height > 1)
		{
			const auto sourceLevel = result._Levels.back();
			const uint32_t mipWidth = std::max(1u, width / 2), mipHeight = std::max(1u, height / 2);
			const size_t offset = result._Pixels.size();
			if (size_t(mipWidth) * mipHeight > (result._Pixels.max_size() - offset) / 4)
			{
				throw std::length_error("Mip chain overflow");
			}
			result._Pixels.resize(offset + size_t(mipWidth) * mipHeight * 4);
			for (uint32_t y = 0; y < mipHeight; ++y)
			{
				for (uint32_t x = 0; x < mipWidth; ++x)
				{
					const double x0 = double(x) * width / mipWidth, x1 = double(x + 1) * width / mipWidth,
								 y0 = double(y) * height / mipHeight, y1 = double(y + 1) * height / mipHeight;
					std::array<double, 4> weightedSum{};
					double totalWeight = 0;
					for (uint32_t sourceY = uint32_t(y0); sourceY < std::min(height, uint32_t(std::ceil(y1)));
						 ++sourceY)
					{
						for (uint32_t sourceX = uint32_t(x0); sourceX < std::min(width, uint32_t(std::ceil(x1)));
							 ++sourceX)
						{
							const double sampleWeight =
								(std::min(x1, double(sourceX + 1)) - std::max(x0, double(sourceX))) *
								(std::min(y1, double(sourceY + 1)) - std::max(y0, double(sourceY)));
							const size_t pixelOffset = sourceLevel._Offset + (size_t(sourceY) * width + sourceX) * 4;
							std::array<double, 4> sample{};
							for (int channel = 0; channel < 4; ++channel)
							{
								sample[channel] = result._Pixels[pixelOffset + channel] / 255.;
							}
							if (kind == TextureMipKind::Color || kind == TextureMipKind::Emission)
							{
								for (int channel = 0; channel < 3; ++channel)
								{
									sample[channel] = DecodeSRGB(float(sample[channel])) *
													  (kind == TextureMipKind::Color ? sample[3] : 1);
								}
							}
							if (kind == TextureMipKind::Roughness)
							{
								for (auto& channel : sample)
								{
									channel *= channel;
								}
							}
							if (kind == TextureMipKind::Normal)
							{
								double length = 0;
								for (int channel = 0; channel < 3; ++channel)
								{
									sample[channel] = sample[channel] * 2 - 1;
									length += sample[channel] * sample[channel];
								}
								length = std::sqrt(length);
								for (int channel = 0; channel < 3; ++channel)
								{
									sample[channel] =
										(length > 1e-8 ? sample[channel] / length : (channel == 2 ? 1 : 0)) * sample[3];
								}
							}
							for (int channel = 0; channel < 4; ++channel)
							{
								weightedSum[channel] += sample[channel] * sampleWeight;
							}
							totalWeight += sampleWeight;
						}
					}
					for (auto& channel : weightedSum)
					{
						channel /= totalWeight;
					}
					if (kind == TextureMipKind::Color)
					{
						for (int channel = 0; channel < 3; ++channel)
						{
							weightedSum[channel] =
								EncodeSRGB(float(weightedSum[3] > 1e-8 ? weightedSum[channel] / weightedSum[3] : 0));
						}
					}
					if (kind == TextureMipKind::Emission)
					{
						for (int channel = 0; channel < 3; ++channel)
						{
							weightedSum[channel] = EncodeSRGB(float(weightedSum[channel]));
						}
					}
					if (kind == TextureMipKind::Roughness)
					{
						for (auto& channel : weightedSum)
						{
							channel = std::sqrt(channel);
						}
					}
					if (kind == TextureMipKind::Normal)
					{
						double length = std::sqrt(weightedSum[0] * weightedSum[0] + weightedSum[1] * weightedSum[1] +
												  weightedSum[2] * weightedSum[2]);
						for (int channel = 0; channel < 3; ++channel)
						{
							weightedSum[channel] =
								(length > 1e-8 ? weightedSum[channel] / length : (channel == 2 ? 1 : 0)) * .5 + .5;
						}
						weightedSum[3] = std::clamp(length, 0., 1.);
					}
					const size_t pixelOffset = offset + (size_t(y) * mipWidth + x) * 4;
					for (int channel = 0; channel < 4; ++channel)
					{
						result._Pixels[pixelOffset + channel] = ToUNorm8(weightedSum[channel]);
					}
				}
			}
			result._Levels.push_back({mipWidth, mipHeight, offset});
			width = mipWidth;
			height = mipHeight;
		}
		return result;
	}
} // namespace DRender::TextureMips
